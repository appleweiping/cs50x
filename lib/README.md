# CS50 library shim (`cs50.h` / `cs50.c`)

The official [CS50 library](https://github.com/cs50/libcs50) (`libcs50`) is normally
pre-installed in the CS50 codespace and provides helpers like `get_string`, `get_int`,
and `get_long`, plus the `string` typedef. It is **not** available on this Windows/MSYS2
build machine, so this directory vendors a small, faithful re-implementation of just the
subset the CS50x problem sets use.

The public API — function names, the `string` typedef, prompt formatting, and the
"reprompt on invalid input" behaviour — matches the real library exactly. That means the
solutions under `pset1/`…`pset5/` are the genuine CS50 code and compile unchanged against
either this shim or the official library.

## What is implemented

| Function | Behaviour |
|---|---|
| `get_string(fmt, ...)` | Reads a full line, returns it sans newline; `NULL` on EOF. |
| `get_char(fmt, ...)`   | Reprompts until input is exactly one character. |
| `get_int(fmt, ...)`    | Reprompts until input is a valid `int`. |
| `get_long(fmt, ...)`   | Reprompts until input is a valid `long`. |
| `get_float(fmt, ...)`  | Reprompts until input is a valid `float`. |
| `get_double(fmt, ...)` | Reprompts until input is a valid `double`. |

Returned strings are tracked internally and freed automatically via an `atexit`
handler, exactly like the real library — pset code does not free them.

## How the psets compile against it

Each C pset has a `Makefile` that adds `-I../../lib` and compiles `../../lib/cs50.c`
alongside the solution, e.g.:

```make
mario: mario.c ../../lib/cs50.c
	gcc -std=c11 -Wall -I../../lib -o mario mario.c ../../lib/cs50.c
```
