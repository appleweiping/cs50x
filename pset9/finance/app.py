"""C$50 Finance — a Flask app for simulated stock trading (CS50x Problem Set 9).

Users can register, log in, get quotes, buy and sell shares (backed by a virtual $10,000
cash balance), see their portfolio, and view transaction history. Data is stored in a
local SQLite database (finance.db). The cs50 Python SQL wrapper is not installed on this
build machine, so a tiny sqlite3-based query helper is used instead — the SQL is identical.
"""

import os
import sqlite3

from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application.
app = Flask(__name__)

# Custom filter.
app.jinja_env.filters["usd"] = usd

# Configure session to use the filesystem (instead of signed cookies).
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Path to the SQLite database (overridable via FINANCE_DB, e.g. for tests).
DB_PATH = os.environ.get("FINANCE_DB", os.path.join(os.path.dirname(__file__), "finance.db"))


def get_db():
    """Open a SQLite connection with dict-like row access."""
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    conn.execute("PRAGMA foreign_keys = ON")
    return conn


def query(sql, params=(), *, fetch=None):
    """Run a SQL statement. `fetch` in {None, 'one', 'all'} controls the return value; for
    INSERT the new lastrowid is returned."""
    conn = get_db()
    try:
        cur = conn.execute(sql, params)
        if fetch == "one":
            row = cur.fetchone()
            result = dict(row) if row else None
        elif fetch == "all":
            result = [dict(r) for r in cur.fetchall()]
        else:
            result = cur.lastrowid
        conn.commit()
        return result
    finally:
        conn.close()


def init_db():
    """Create the schema on first run (idempotent)."""
    conn = get_db()
    conn.executescript(
        """
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            hash TEXT NOT NULL,
            cash NUMERIC NOT NULL DEFAULT 10000.00
        );

        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            symbol TEXT NOT NULL,
            shares INTEGER NOT NULL,
            price NUMERIC NOT NULL,
            timestamp DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(user_id) REFERENCES users(id)
        );
        """
    )
    conn.commit()
    conn.close()


init_db()


@app.after_request
def after_request(response):
    """Ensure responses aren't cached."""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks."""
    user_id = session["user_id"]

    # Aggregate holdings per symbol.
    holdings = query(
        """
        SELECT symbol, SUM(shares) AS shares
          FROM transactions
         WHERE user_id = ?
         GROUP BY symbol
        HAVING SUM(shares) > 0
        """,
        (user_id,),
        fetch="all",
    )

    # Enrich each holding with the current price and value.
    stocks = []
    holdings_total = 0.0
    for h in holdings:
        quote = lookup(h["symbol"])
        price = quote["price"]
        value = price * h["shares"]
        holdings_total += value
        stocks.append(
            {"symbol": h["symbol"], "shares": h["shares"], "price": price, "value": value}
        )

    cash = query("SELECT cash FROM users WHERE id = ?", (user_id,), fetch="one")["cash"]
    grand_total = cash + holdings_total

    return render_template("index.html", stocks=stocks, cash=cash, total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html")

    symbol = request.form.get("symbol", "").strip()
    shares_raw = request.form.get("shares", "")

    if not symbol:
        return apology("must provide symbol")

    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol")

    # Validate share count is a positive integer.
    try:
        shares = int(shares_raw)
    except (TypeError, ValueError):
        return apology("shares must be a positive integer")
    if shares <= 0:
        return apology("shares must be a positive integer")

    user_id = session["user_id"]
    cash = query("SELECT cash FROM users WHERE id = ?", (user_id,), fetch="one")["cash"]
    cost = quote["price"] * shares
    if cost > cash:
        return apology("can't afford")

    # Record the purchase and deduct cash.
    query(
        "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
        (user_id, quote["symbol"], shares, quote["price"]),
    )
    query("UPDATE users SET cash = cash - ? WHERE id = ?", (cost, user_id))

    flash(f"Bought {shares} share(s) of {quote['symbol']} for {usd(cost)}!")
    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    user_id = session["user_id"]
    rows = query(
        """
        SELECT symbol, shares, price, timestamp
          FROM transactions
         WHERE user_id = ?
         ORDER BY timestamp DESC, id DESC
        """,
        (user_id,),
        fetch="all",
    )
    return render_template("history.html", transactions=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""
    session.clear()

    if request.method == "GET":
        return render_template("login.html")

    username = request.form.get("username", "")
    password = request.form.get("password", "")

    if not username:
        return apology("must provide username", 403)
    if not password:
        return apology("must provide password", 403)

    user = query("SELECT * FROM users WHERE username = ?", (username,), fetch="one")
    if user is None or not check_password_hash(user["hash"], password):
        return apology("invalid username and/or password", 403)

    session["user_id"] = user["id"]
    return redirect("/")


@app.route("/logout")
def logout():
    """Log user out."""
    session.clear()
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    symbol = request.form.get("symbol", "").strip()
    if not symbol:
        return apology("must provide symbol")

    result = lookup(symbol)
    if result is None:
        return apology("invalid symbol")

    return render_template("quoted.html", quote=result)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "GET":
        return render_template("register.html")

    username = request.form.get("username", "")
    password = request.form.get("password", "")
    confirmation = request.form.get("confirmation", "")

    if not username:
        return apology("must provide username", 400)
    if not password:
        return apology("must provide password", 400)
    if password != confirmation:
        return apology("passwords do not match", 400)

    # Enforce unique usernames.
    existing = query("SELECT id FROM users WHERE username = ?", (username,), fetch="one")
    if existing is not None:
        return apology("username already taken", 400)

    user_id = query(
        "INSERT INTO users (username, hash) VALUES (?, ?)",
        (username, generate_password_hash(password)),
    )

    # Log the new user in.
    session["user_id"] = user_id
    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    user_id = session["user_id"]

    # Symbols the user currently owns (positive holdings).
    owned = query(
        """
        SELECT symbol
          FROM transactions
         WHERE user_id = ?
         GROUP BY symbol
        HAVING SUM(shares) > 0
        """,
        (user_id,),
        fetch="all",
    )
    symbols = [row["symbol"] for row in owned]

    if request.method == "GET":
        return render_template("sell.html", symbols=symbols)

    symbol = request.form.get("symbol", "").strip().upper()
    shares_raw = request.form.get("shares", "")

    if not symbol:
        return apology("must provide symbol")
    if symbol not in symbols:
        return apology("you do not own this stock")

    try:
        shares = int(shares_raw)
    except (TypeError, ValueError):
        return apology("shares must be a positive integer")
    if shares <= 0:
        return apology("shares must be a positive integer")

    # Confirm the user owns enough shares.
    held = query(
        "SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ?",
        (user_id, symbol),
        fetch="one",
    )["shares"]
    if shares > held:
        return apology("too many shares")

    quote = lookup(symbol)
    proceeds = quote["price"] * shares

    # Record the sale (negative shares) and credit cash.
    query(
        "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
        (user_id, symbol, -shares, quote["price"]),
    )
    query("UPDATE users SET cash = cash + ? WHERE id = ?", (proceeds, user_id))

    flash(f"Sold {shares} share(s) of {symbol} for {usd(proceeds)}!")
    return redirect("/")


if __name__ == "__main__":
    app.run(debug=True, port=5000)
