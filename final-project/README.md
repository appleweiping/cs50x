# FlashLearn — CS50x Final Project

FlashLearn is a spaced-repetition flashcards web app. Users register, build decks of
cards, and study them in a review mode that schedules each card with a simplified
**Leitner system**: answering a card correctly promotes it to a higher "box" with a longer
review interval, while answering incorrectly resets it to box 1 (due immediately).

## Features

- User accounts with hashed passwords (register / log in / log out).
- Deck management: create, view, and delete decks (per-user, isolated).
- Card management: add and delete cards (front = question, back = answer).
- Review mode with a "show answer" reveal and right/wrong grading.
- Leitner spaced repetition: box 1→5 with intervals of 0, 1, 3, 7, 14 days.
- SQLite storage, Flask + Bootstrap 5 UI.

## Video demo

A local screen recording of the flow (register → create deck → add cards → study) can be
produced by running the app; the walkthrough is described in `results/` at the repo root.

## Run it

```bash
# From the repo's shared venv (Flask installed there):
python -m pip install -r requirements.txt   # or reuse the shared venv
python app.py                               # serves on http://localhost:5001
```

## Test it

```bash
python test_app.py    # 20/20 end-to-end tests (auth, CRUD, ownership, Leitner scheduling)
```

## How the spaced repetition works

Each card has a `box` (1–5) and a `due_date`. On review, only cards with
`due_date <= today` are shown. Grading updates the card:

| Result  | New box            | Next due (days) |
|---------|--------------------|-----------------|
| Correct | min(box + 1, 5)    | 0 / 1 / 3 / 7 / 14 by box |
| Wrong   | 1                  | 0 (today)       |

## Tech stack

Python 3.11, Flask, SQLite (`sqlite3`), Werkzeug password hashing, Bootstrap 5.

## Note

This is the final project for Harvard **CS50x**, built independently for the
[csdiy.wiki](https://csdiy.wiki/) catalog. Released under the MIT License.
