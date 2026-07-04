# Harvard CS50x — Solutions

> Complete solutions to Harvard's **CS50x: Introduction to Computer Science** — every
> problem set in C, Python, and SQL, plus a final project — an independent, from-scratch
> implementation, part of a [csdiy.wiki](https://csdiy.wiki/) full-catalog build.

![status](https://img.shields.io/badge/status-complete-brightgreen)
![language](https://img.shields.io/badge/C%20·%20Python%20·%20SQL-informational)
![license](https://img.shields.io/badge/license-MIT-blue)

## Overview

CS50x is Harvard's introduction to computer science and the art of programming. This repo
contains working, verified solutions to all nine problem sets and a final project. The C
problem sets (psets 1–5) are the genuine CS50 code; because the official `libcs50` and the
online `check50` autograder are not available on this Windows/MSYS2 build machine, every
program was instead **run on real inputs** and its output captured to [`results/`](results/).
A small faithful re-implementation of the CS50 I/O helpers lives in [`lib/`](lib/).

## Results (measured on Windows 11, MSYS2 GCC 14.2, Python 3.11, CPU-only)

| Problem set | Problems | Result (measured) |
|---|---|---|
| **pset1** | mario, cash, credit | mario pyramid correct; cash greedy 0.41→4; credit VISA/AMEX/MC/INVALID all correct |
| **pset2** | scrabble, readability, caesar, substitution | Coleman-Liau grades match; ROT13 `Hello, World`→`Uryyb, Jbeyq` |
| **pset3** | plurality, runoff, tideman | runoff→Charlie, tideman→Alice (match CS50 examples); Condorcet cycle handled |
| **pset4** | volume, filter, recover | volume peak ×2 exact; reflect == numpy fliplr; 3 JPEGs recovered |
| **pset5** | inheritance, speller | speller on lalaland.txt → **955 misspellings** (matches CS50), load 0.17 s |
| **pset6** | hello, mario, cash, credit, readability, dna | Python ports match C; DNA → Bob / No match / Alice |
| **pset7** | songs (8), movies (13), fiftyville | all queries verified on real DBs; **fiftyville → Bruce / New York City / Robin** |
| **pset8** | homepage | 4-page Bootstrap site; quiz grading 2/2, 1/2, 0/2 |
| **pset9** | finance | **21/21** end-to-end tests (register/quote/buy/sell/history) |
| **final** | FlashLearn | **20/20** end-to-end tests (auth, CRUD, Leitner spaced repetition) |

Sample filter outputs (grayscale / edges / reflect) and recovered JPEGs are in
[`results/pset4_samples/`](results/pset4_samples/).

## Implemented assignments

- [x] **pset1** — Mario (more), Cash (greedy), Credit (Luhn)
- [x] **pset2** — Scrabble, Readability (Coleman-Liau), Caesar, Substitution
- [x] **pset3** — Plurality, Runoff (IRV), Tideman (Condorcet / ranked pairs)
- [x] **pset4** — Volume, Filter (grayscale/reflect/blur/edges), Recover
- [x] **pset5** — Inheritance, Speller (hash-table dictionary)
- [x] **pset6** — Hello, Mario, Cash, Credit, Readability, DNA (Python)
- [x] **pset7** — Songs, Movies, Fiftyville (SQL)
- [x] **pset8** — Homepage (HTML/CSS/JS/Bootstrap)
- [x] **pset9** — Finance (Flask stock-trading app)
- [x] **Final project** — FlashLearn, a spaced-repetition flashcards web app

## Project structure

```
cs50x/
├── lib/                cs50.h / cs50.c  (CS50 I/O shim for psets 1–5)
├── pset1/  mario  cash  credit
├── pset2/  scrabble  readability  caesar  substitution
├── pset3/  plurality  runoff  tideman
├── pset4/  volume  filter  recover
├── pset5/  inheritance  speller
├── pset6/  hello  mario  cash  credit  readability  dna   (Python)
├── pset7/  songs  movies  fiftyville                      (SQL)
├── pset8/  homepage                                       (web)
├── pset9/  finance                                        (Flask)
├── final-project/  FlashLearn                             (Flask)
└── results/        captured run outputs (evidence)
```

## How to run

```bash
# --- C problem sets (MSYS2 GCC) --- each folder has a Makefile:
cd pset1/mario && make && echo 4 | ./mario

# --- Python (pset6) ---
python pset6/dna/dna.py pset6/dna/databases/small.csv pset6/dna/sequences/1.txt   # -> Bob

# --- SQL (pset7) ---   (movies.db is large: pset7/movies/download.sh fetches it)
sqlite3 pset7/songs/songs.db < pset7/songs/6.sql        # Post Malone's songs
sqlite3 pset7/fiftyville/fiftyville.db < pset7/fiftyville/log.sql

# --- Flask apps (pset9 & final) --- use the shared csdiy venv (Flask installed):
#   D:\Project\_csdiy\.venv-ml\Scripts\python.exe
python pset9/finance/test_finance.py     # 21/21
python final-project/test_app.py         # 20/20
python pset9/finance/app.py              # live server on :5000
```

## Verification

`check50`/`submit50` are online-only and unavailable here, so correctness was established
by **running every program on real inputs** and comparing against known-good CS50 outputs:

- **C psets** — compiled with `gcc -std=c11 -Wall -Wextra`; each program run on canonical
  inputs (e.g. speller → 955 misspellings on `lalaland.txt`, exactly CS50's figure).
- **pset4** — verified numerically with NumPy/PIL (volume peak doubles; reflect equals a
  horizontal flip; recovered files open as valid JPEGs).
- **pset7** — every query executed against the real CS50 databases (songs 100 tracks,
  movies 433k titles, fiftyville mystery solved to Bruce / New York City / Robin).
- **pset9 & final** — Flask test-client suites (`test_finance.py` 21/21, `test_app.py`
  20/20) plus live-server smoke tests.

Full captured output for each pset is in [`results/`](results/).

## Tech stack

C (MSYS2 GCC 14.2, C11), Python 3.11, SQLite / SQL, Flask, Werkzeug, Bootstrap 5,
HTML/CSS/JavaScript. NumPy/PIL used only to *verify* the pset4 image/audio outputs.

## Key ideas / what I learned

- Low-level C: pointers, structs, dynamic memory, file I/O, bit-level image/audio
  manipulation, and a hash-table dictionary (speller).
- Algorithms: greedy change-making, Luhn's checksum, instant-runoff and Condorcet
  (ranked-pairs) voting with cycle detection.
- Data & SQL: joins, subqueries, aggregation, and multi-step investigative querying.
- Web: Flask routing, sessions, password hashing, SQLite persistence, and a Leitner
  spaced-repetition scheduler.

## Credits & license

Based on the problem sets of **CS50x: Introduction to Computer Science** by David J. Malan
and Harvard University. This repository is an independent educational reimplementation; all
course materials, datasets, and specifications belong to their original authors. The CS50
duck and CS50 name are trademarks of Harvard's CS50. Original code in this repo is released
under the [MIT License](LICENSE).
