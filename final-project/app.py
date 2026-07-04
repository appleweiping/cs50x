"""FlashLearn — a spaced-repetition flashcards web app (CS50x final project).

Users register and log in, create decks of flashcards, and study them in a review mode that
schedules each card with a simplified Leitner spaced-repetition algorithm: getting a card
right promotes it to a longer interval box; getting it wrong resets it to box 1. The app is
built with Flask and SQLite and is fully covered by an end-to-end test suite (test_app.py).
"""

import os
import sqlite3
from datetime import date, timedelta

from flask import (
    Flask, flash, g, redirect, render_template, request, session, url_for
)
from werkzeug.security import check_password_hash, generate_password_hash

app = Flask(__name__)
app.secret_key = os.environ.get("FLASHLEARN_SECRET", "dev-secret-change-me")

DB_PATH = os.environ.get("FLASHLEARN_DB", os.path.join(os.path.dirname(__file__), "flashlearn.db"))

# Leitner box -> days until the card is due again after a correct answer.
BOX_INTERVALS = {1: 0, 2: 1, 3: 3, 4: 7, 5: 14}
MAX_BOX = 5


# --- Database helpers ----------------------------------------------------------------
def get_db():
    if "db" not in g:
        g.db = sqlite3.connect(DB_PATH)
        g.db.row_factory = sqlite3.Row
        g.db.execute("PRAGMA foreign_keys = ON")
    return g.db


@app.teardown_appcontext
def close_db(exception=None):
    db = g.pop("db", None)
    if db is not None:
        db.close()


def init_db():
    conn = sqlite3.connect(DB_PATH)
    conn.executescript(
        """
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            hash TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS decks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id)
        );

        CREATE TABLE IF NOT EXISTS cards (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            deck_id INTEGER NOT NULL,
            front TEXT NOT NULL,
            back TEXT NOT NULL,
            box INTEGER NOT NULL DEFAULT 1,
            due_date TEXT NOT NULL DEFAULT (date('now')),
            FOREIGN KEY(deck_id) REFERENCES decks(id)
        );
        """
    )
    conn.commit()
    conn.close()


init_db()


# --- Auth ----------------------------------------------------------------------------
def login_required(view):
    from functools import wraps

    @wraps(view)
    def wrapped(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect(url_for("login"))
        return view(*args, **kwargs)

    return wrapped


def current_user_id():
    return session.get("user_id")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    username = request.form.get("username", "").strip()
    password = request.form.get("password", "")
    confirmation = request.form.get("confirmation", "")

    if not username:
        flash("Username is required.")
        return render_template("register.html"), 400
    if not password:
        flash("Password is required.")
        return render_template("register.html"), 400
    if password != confirmation:
        flash("Passwords do not match.")
        return render_template("register.html"), 400

    db = get_db()
    if db.execute("SELECT 1 FROM users WHERE username = ?", (username,)).fetchone():
        flash("Username already taken.")
        return render_template("register.html"), 400

    cur = db.execute(
        "INSERT INTO users (username, hash) VALUES (?, ?)",
        (username, generate_password_hash(password)),
    )
    db.commit()
    session.clear()
    session["user_id"] = cur.lastrowid
    session["username"] = username
    return redirect(url_for("index"))


@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "GET":
        return render_template("login.html")

    username = request.form.get("username", "").strip()
    password = request.form.get("password", "")

    db = get_db()
    user = db.execute("SELECT * FROM users WHERE username = ?", (username,)).fetchone()
    if user is None or not check_password_hash(user["hash"], password):
        flash("Invalid username and/or password.")
        return render_template("login.html"), 403

    session.clear()
    session["user_id"] = user["id"]
    session["username"] = user["username"]
    return redirect(url_for("index"))


@app.route("/logout")
def logout():
    session.clear()
    return redirect(url_for("login"))


# --- Decks ---------------------------------------------------------------------------
@app.route("/")
@login_required
def index():
    db = get_db()
    decks = db.execute(
        """
        SELECT d.id, d.name,
               COUNT(c.id) AS total,
               SUM(CASE WHEN c.due_date <= date('now') THEN 1 ELSE 0 END) AS due
          FROM decks d
          LEFT JOIN cards c ON c.deck_id = d.id
         WHERE d.user_id = ?
         GROUP BY d.id
         ORDER BY d.name
        """,
        (current_user_id(),),
    ).fetchall()
    return render_template("index.html", decks=decks)


@app.route("/decks/create", methods=["POST"])
@login_required
def create_deck():
    name = request.form.get("name", "").strip()
    if not name:
        flash("Deck name is required.")
        return redirect(url_for("index"))
    db = get_db()
    db.execute("INSERT INTO decks (user_id, name) VALUES (?, ?)", (current_user_id(), name))
    db.commit()
    flash(f"Created deck '{name}'.")
    return redirect(url_for("index"))


def _owned_deck(deck_id):
    db = get_db()
    return db.execute(
        "SELECT * FROM decks WHERE id = ? AND user_id = ?", (deck_id, current_user_id())
    ).fetchone()


@app.route("/decks/<int:deck_id>")
@login_required
def view_deck(deck_id):
    deck = _owned_deck(deck_id)
    if deck is None:
        return "Not found", 404
    db = get_db()
    cards = db.execute(
        "SELECT * FROM cards WHERE deck_id = ? ORDER BY id", (deck_id,)
    ).fetchall()
    return render_template("deck.html", deck=deck, cards=cards)


@app.route("/decks/<int:deck_id>/delete", methods=["POST"])
@login_required
def delete_deck(deck_id):
    if _owned_deck(deck_id) is None:
        return "Not found", 404
    db = get_db()
    db.execute("DELETE FROM cards WHERE deck_id = ?", (deck_id,))
    db.execute("DELETE FROM decks WHERE id = ?", (deck_id,))
    db.commit()
    flash("Deck deleted.")
    return redirect(url_for("index"))


# --- Cards ---------------------------------------------------------------------------
@app.route("/decks/<int:deck_id>/cards/create", methods=["POST"])
@login_required
def create_card(deck_id):
    if _owned_deck(deck_id) is None:
        return "Not found", 404
    front = request.form.get("front", "").strip()
    back = request.form.get("back", "").strip()
    if not front or not back:
        flash("Both front and back are required.")
        return redirect(url_for("view_deck", deck_id=deck_id))
    db = get_db()
    db.execute(
        "INSERT INTO cards (deck_id, front, back) VALUES (?, ?, ?)", (deck_id, front, back)
    )
    db.commit()
    flash("Card added.")
    return redirect(url_for("view_deck", deck_id=deck_id))


@app.route("/cards/<int:card_id>/delete", methods=["POST"])
@login_required
def delete_card(card_id):
    db = get_db()
    card = db.execute(
        """
        SELECT c.*, d.user_id AS owner FROM cards c
          JOIN decks d ON d.id = c.deck_id
         WHERE c.id = ?
        """,
        (card_id,),
    ).fetchone()
    if card is None or card["owner"] != current_user_id():
        return "Not found", 404
    deck_id = card["deck_id"]
    db.execute("DELETE FROM cards WHERE id = ?", (card_id,))
    db.commit()
    flash("Card deleted.")
    return redirect(url_for("view_deck", deck_id=deck_id))


# --- Review (spaced repetition) ------------------------------------------------------
@app.route("/decks/<int:deck_id>/review")
@login_required
def review(deck_id):
    if _owned_deck(deck_id) is None:
        return "Not found", 404
    db = get_db()
    card = db.execute(
        """
        SELECT * FROM cards
         WHERE deck_id = ? AND due_date <= date('now')
         ORDER BY due_date, id
         LIMIT 1
        """,
        (deck_id,),
    ).fetchone()
    return render_template("review.html", deck_id=deck_id, card=card)


@app.route("/cards/<int:card_id>/grade", methods=["POST"])
@login_required
def grade_card(card_id):
    correct = request.form.get("result") == "correct"
    db = get_db()
    card = db.execute(
        """
        SELECT c.*, d.user_id AS owner FROM cards c
          JOIN decks d ON d.id = c.deck_id
         WHERE c.id = ?
        """,
        (card_id,),
    ).fetchone()
    if card is None or card["owner"] != current_user_id():
        return "Not found", 404

    # Leitner scheduling: correct -> promote a box (max 5); wrong -> reset to box 1.
    if correct:
        new_box = min(card["box"] + 1, MAX_BOX)
    else:
        new_box = 1
    interval = BOX_INTERVALS[new_box]
    new_due = (date.today() + timedelta(days=interval)).isoformat()

    db.execute(
        "UPDATE cards SET box = ?, due_date = ? WHERE id = ?", (new_box, new_due, card_id)
    )
    db.commit()
    return redirect(url_for("review", deck_id=card["deck_id"]))


if __name__ == "__main__":
    app.run(debug=True, port=5001)
