"""End-to-end tests for FlashLearn (CS50x final project).

Covers auth, deck & card CRUD, ownership isolation between users, and the Leitner
spaced-repetition scheduling. Run:  python test_app.py
"""

import os
import sys
import tempfile
from datetime import date, timedelta

_tmp = tempfile.mkdtemp()
os.environ["FLASHLEARN_DB"] = os.path.join(_tmp, "test.db")

import app as fl  # noqa: E402

fl.DB_PATH = os.environ["FLASHLEARN_DB"]
fl.init_db()
fl.app.config["TESTING"] = True

passed = failed = 0


def check(desc, cond):
    global passed, failed
    if cond:
        passed += 1
        print(f"PASS  {desc}")
    else:
        failed += 1
        print(f"FAIL  {desc}")


def new_client():
    return fl.app.test_client()


def q(sql, params=()):
    conn = fl.sqlite3.connect(fl.DB_PATH)
    conn.row_factory = fl.sqlite3.Row
    try:
        return [dict(r) for r in conn.execute(sql, params).fetchall()]
    finally:
        conn.close()


# --- Auth ---------------------------------------------------------------------------
c = new_client()
r = c.get("/", follow_redirects=False)
check("anonymous / redirects to /login", r.status_code == 302 and "/login" in r.headers["Location"])

r = c.post("/register", data={"username": "amy", "password": "pw", "confirmation": "pw"}, follow_redirects=False)
check("register amy -> redirect to /", r.status_code == 302 and r.headers["Location"].endswith("/"))

r = c.post("/register", data={"username": "amy", "password": "pw", "confirmation": "pw"})
check("duplicate username rejected (400)", r.status_code == 400)

r = c.post("/register", data={"username": "x", "password": "a", "confirmation": "b"})
check("password mismatch rejected (400)", r.status_code == 400)

# --- Deck CRUD ----------------------------------------------------------------------
r = c.post("/decks/create", data={"name": "Spanish"}, follow_redirects=True)
check("create deck 'Spanish'", b"Spanish" in r.data)
deck_id = q("SELECT id FROM decks WHERE name='Spanish'")[0]["id"]

r = c.post("/decks/create", data={"name": ""}, follow_redirects=True)
check("empty deck name rejected (flash, no new deck)", len(q("SELECT id FROM decks")) == 1)

# --- Card CRUD ----------------------------------------------------------------------
c.post(f"/decks/{deck_id}/cards/create", data={"front": "hola", "back": "hello"})
c.post(f"/decks/{deck_id}/cards/create", data={"front": "gato", "back": "cat"})
cards = q("SELECT * FROM cards WHERE deck_id=?", (deck_id,))
check("two cards created", len(cards) == 2)
check("new cards start in box 1", all(c_["box"] == 1 for c_ in cards))
check("new cards due today (<= today)", all(c_["due_date"] <= date.today().isoformat() for c_ in cards))

r = c.post(f"/decks/{deck_id}/cards/create", data={"front": "", "back": "x"}, follow_redirects=True)
check("card with empty front rejected", len(q("SELECT * FROM cards WHERE deck_id=?", (deck_id,))) == 2)

# --- Review scheduling (Leitner) ----------------------------------------------------
card_id = cards[0]["id"]
# A correct answer promotes box 1 -> 2 and schedules 1 day out.
c.post(f"/cards/{card_id}/grade", data={"result": "correct"})
row = q("SELECT box, due_date FROM cards WHERE id=?", (card_id,))[0]
expected_due = (date.today() + timedelta(days=fl.BOX_INTERVALS[2])).isoformat()
check("correct answer promotes box 1 -> 2", row["box"] == 2)
check("box 2 due date = today + 1 day", row["due_date"] == expected_due)

# Two more correct answers: 2 -> 3 -> 4.
c.post(f"/cards/{card_id}/grade", data={"result": "correct"})
c.post(f"/cards/{card_id}/grade", data={"result": "correct"})
check("box climbs to 4 after 3 correct", q("SELECT box FROM cards WHERE id=?", (card_id,))[0]["box"] == 4)

# A wrong answer resets to box 1, due today.
c.post(f"/cards/{card_id}/grade", data={"result": "wrong"})
row = q("SELECT box, due_date FROM cards WHERE id=?", (card_id,))[0]
check("wrong answer resets to box 1", row["box"] == 1)
check("reset card due today", row["due_date"] == date.today().isoformat())

# review view returns a due card (200) and, importantly, the card's front text.
r = c.get(f"/decks/{deck_id}/review")
check("review page shows a due card", r.status_code == 200 and (b"hola" in r.data or b"gato" in r.data))

# --- Ownership isolation ------------------------------------------------------------
c2 = new_client()
c2.post("/register", data={"username": "ben", "password": "pw", "confirmation": "pw"})
r = c2.get(f"/decks/{deck_id}")
check("other user cannot view amy's deck (404)", r.status_code == 404)
r = c2.post(f"/cards/{card_id}/delete")
check("other user cannot delete amy's card (404)", r.status_code == 404)

# --- Delete ------------------------------------------------------------------------
r = c.post(f"/cards/{card_id}/delete", follow_redirects=True)
check("owner deletes a card", len(q("SELECT * FROM cards WHERE deck_id=?", (deck_id,))) == 1)
r = c.post(f"/decks/{deck_id}/delete", follow_redirects=True)
check("owner deletes deck (and its cards cascade)", len(q("SELECT * FROM decks")) == 0 and len(q("SELECT * FROM cards")) == 0)

print(f"\n{passed} passed, {failed} failed")
sys.exit(0 if failed == 0 else 1)
