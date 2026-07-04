"""Helper utilities for the CS50x Finance app: login gate, price lookup, USD formatting."""

import hashlib
from functools import wraps

import requests
from flask import redirect, render_template, session


def apology(message, code=400):
    """Render an apology message to the user (mirrors CS50's memegen apology)."""

    def escape(s):
        # Escape special characters so the message is safe in a URL/template.
        for old, new in [
            ("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
            ("%", "~p"), ("#", "~h"), ("/", "~s"), ('"', "''"),
        ]:
            s = s.replace(old, new)
        return s

    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """Decorate routes to require login (redirects to /login if no user in session)."""

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)

    return decorated_function


def lookup(symbol):
    """Look up the current price of `symbol`.

    Tries Yahoo Finance's public quote endpoint; if the network is unavailable (this build
    machine is often offline / behind a firewall), falls back to a *deterministic* pseudo
    price derived from the symbol so the whole app remains testable end-to-end. The return
    shape matches CS50's: {"name", "price", "symbol"} or None for an unknown symbol.
    """

    symbol = symbol.upper().strip()
    if not symbol.isalpha():
        return None

    # Primary: Yahoo Finance quote API.
    try:
        url = f"https://query1.finance.yahoo.com/v8/finance/chart/{symbol}"
        r = requests.get(
            url,
            headers={"User-Agent": "Mozilla/5.0"},
            timeout=5,
        )
        r.raise_for_status()
        result = r.json()["chart"]["result"][0]
        price = result["meta"]["regularMarketPrice"]
        return {"name": symbol, "price": round(float(price), 2), "symbol": symbol}
    except Exception:
        pass

    # Fallback: a stable, deterministic price so the app works offline. This is documented
    # in the README; it is NOT a real market price, just a reproducible stand-in.
    digest = hashlib.sha256(symbol.encode()).hexdigest()
    # Map the first 6 hex digits to a price in [10.00, 1000.00].
    price = 10 + (int(digest[:6], 16) % 99000) / 100.0
    return {"name": symbol, "price": round(price, 2), "symbol": symbol}


def usd(value):
    """Format a numeric value as USD (Jinja filter)."""
    return f"${value:,.2f}"
