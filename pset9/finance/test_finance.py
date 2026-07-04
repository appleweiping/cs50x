"""End-to-end tests for the C$50 Finance app using Flask's test client.

Exercises register / quote / buy / index / sell / history / login / logout and the main
error paths, asserting on real HTTP responses and database state. Run:

    python test_finance.py
"""

import os
import sys
import tempfile

# Use a throwaway database so tests never touch a real finance.db.
_tmp = tempfile.mkdtemp()
os.environ["FINANCE_DB"] = os.path.join(_tmp, "test.db")

import app as finance  # noqa: E402

# Point the app at the temp DB and (re)create the schema there.
finance.DB_PATH = os.environ["FINANCE_DB"]
finance.init_db()

finance.app.config["TESTING"] = True
finance.app.config["WTF_CSRF_ENABLED"] = False
finance.app.config["SESSION_TYPE"] = "filesystem"

client = finance.app.test_client()

passed = 0
failed = 0


def check(desc, condition):
    global passed, failed
    if condition:
        passed += 1
        print(f"PASS  {desc}")
    else:
        failed += 1
        print(f"FAIL  {desc}")


# --- Anonymous access is gated ------------------------------------------------------
r = client.get("/", follow_redirects=False)
check("GET / redirects to /login when logged out", r.status_code == 302 and "/login" in r.headers["Location"])

# --- Registration -------------------------------------------------------------------
r = client.post("/register", data={"username": "alice", "password": "pw123", "confirmation": "pw123"}, follow_redirects=False)
check("register alice logs in (redirect to /)", r.status_code == 302 and r.headers["Location"] == "/")

r = client.post("/register", data={"username": "alice", "password": "pw123", "confirmation": "pw123"})
check("duplicate username rejected (400)", r.status_code == 400)

r = client.post("/register", data={"username": "bob", "password": "a", "confirmation": "b"})
check("mismatched passwords rejected (400)", r.status_code == 400)

# --- Portfolio starts with $10,000 cash --------------------------------------------
r = client.get("/")
check("index shows starting cash $10,000.00", b"$10,000.00" in r.data)

# --- Quote --------------------------------------------------------------------------
r = client.post("/quote", data={"symbol": "AAPL"})
check("quote AAPL returns a price page", r.status_code == 200 and b"AAPL" in r.data and b"$" in r.data)

r = client.post("/quote", data={"symbol": "123"})
check("quote invalid symbol rejected", r.status_code == 400)

# --- Buy ----------------------------------------------------------------------------
price = finance.lookup("NFLX")["price"]
r = client.post("/buy", data={"symbol": "NFLX", "shares": "5"}, follow_redirects=True)
check("buy 5 NFLX succeeds", r.status_code == 200 and b"Bought" in r.data)

# Cash should have decreased by 5 * price.
row = finance.query("SELECT cash FROM users WHERE username = 'alice'", fetch="one")
expected_cash = round(10000 - 5 * price, 2)
check(f"cash decreased to ~{expected_cash}", abs(row["cash"] - expected_cash) < 0.01)

# Holdings show up on index.
r = client.get("/")
check("index lists NFLX holding of 5 shares", b"NFLX" in r.data)

r = client.post("/buy", data={"symbol": "NFLX", "shares": "-3"})
check("buy negative shares rejected", r.status_code == 400)

r = client.post("/buy", data={"symbol": "NFLX", "shares": "100000"})
check("buy unaffordable rejected (can't afford)", r.status_code == 400)

# --- Sell ---------------------------------------------------------------------------
r = client.post("/sell", data={"symbol": "NFLX", "shares": "2"}, follow_redirects=True)
check("sell 2 NFLX succeeds", r.status_code == 200 and b"Sold" in r.data)

r = client.post("/sell", data={"symbol": "NFLX", "shares": "10"})
check("sell more than owned rejected", r.status_code == 400)

r = client.post("/sell", data={"symbol": "TSLA", "shares": "1"})
check("sell unowned stock rejected", r.status_code == 400)

# 5 bought - 2 sold = 3 shares remain.
held = finance.query(
    "SELECT SUM(shares) AS s FROM transactions t JOIN users u ON u.id=t.user_id "
    "WHERE u.username='alice' AND symbol='NFLX'",
    fetch="one",
)["s"]
check("net NFLX shares held == 3", held == 3)

# --- History ------------------------------------------------------------------------
r = client.get("/history")
check("history shows both buy (+5) and sell (-2) rows", r.status_code == 200 and b"NFLX" in r.data and b"-2" in r.data)

# --- Logout / login -----------------------------------------------------------------
r = client.get("/logout", follow_redirects=False)
check("logout redirects to /", r.status_code == 302)

r = client.get("/", follow_redirects=False)
check("after logout, / redirects to /login", r.status_code == 302 and "/login" in r.headers["Location"])

r = client.post("/login", data={"username": "alice", "password": "wrong"})
check("login with wrong password rejected (403)", r.status_code == 403)

r = client.post("/login", data={"username": "alice", "password": "pw123"}, follow_redirects=False)
check("login with correct password succeeds", r.status_code == 302 and r.headers["Location"] == "/")

print(f"\n{passed} passed, {failed} failed")
sys.exit(0 if failed == 0 else 1)
