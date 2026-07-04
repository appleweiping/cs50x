// Minimal CS50 library shim (get_string/get_int/get_long/get_char/get_float/get_double)
//
// The official cs50 library (libcs50) is not available on this Windows/MSYS2 box, so this
// header + cs50.c provide the small subset of the CS50 I/O API used by CS50x problem sets.
// The public API (function names, `string` typedef, prompt semantics) matches the real
// library so the pset solutions are the genuine CS50 code, unchanged. See lib/README.md.
//
// SPDX-License-Identifier: MIT

#ifndef CS50_H
#define CS50_H

#include <float.h>
#include <limits.h>

/**
 * Our own data type for string variables (a synonym for `char *`), matching CS50's.
 */
typedef char *string;

/**
 * Prompts user for a line of text from standard input and returns it as a string
 * (char *), sans trailing line ending. Returns NULL on EOF. Leading and trailing
 * whitespace is NOT stripped (matching CS50). The returned pointer is tracked and
 * freed automatically at exit.
 */
string get_string(const char *format, ...) __attribute__((format(printf, 1, 2)));

/**
 * Prompts user for a line of text from standard input and returns the equivalent
 * char; if text does not represent a single char, user is reprompted.
 */
char get_char(const char *format, ...) __attribute__((format(printf, 1, 2)));

/**
 * Prompts user for a line of text from standard input and returns the equivalent
 * double as precisely as possible; if text does not represent a double, user is
 * reprompted.
 */
double get_double(const char *format, ...) __attribute__((format(printf, 1, 2)));

/**
 * Prompts user for a line of text from standard input and returns the equivalent
 * float as precisely as possible; if text does not represent a float, user is
 * reprompted.
 */
float get_float(const char *format, ...) __attribute__((format(printf, 1, 2)));

/**
 * Prompts user for a line of text from standard input and returns the equivalent
 * int; if text does not represent an int in [-2^31, 2^31 - 1), user is reprompted.
 */
int get_int(const char *format, ...) __attribute__((format(printf, 1, 2)));

/**
 * Prompts user for a line of text from standard input and returns the equivalent
 * long; if text does not represent a long in [-2^63, 2^63 - 1), user is reprompted.
 *
 * NOTE on portability: the real CS50 platform is 64-bit Linux where `long` is 64 bits,
 * which is what pset code (e.g. credit's 16-digit card numbers) relies on. This build
 * machine is Windows/MSYS2 (LLP64) where `long` is only 32 bits. To preserve the CS50
 * semantics unchanged, `get_long` is declared to return a guaranteed-64-bit type here.
 * On a 64-bit-`long` platform this is identical to CS50's `long get_long(...)`.
 */
#include <stdint.h>
int64_t get_long(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif
