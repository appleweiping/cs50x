// Minimal CS50 library shim implementation.
//
// Implements get_string/get_char/get_double/get_float/get_int/get_long with the same
// reprompt-on-invalid-input behaviour as the real CS50 library. Allocated strings are
// tracked and freed via an atexit handler so pset code needn't free them, matching CS50.
//
// SPDX-License-Identifier: MIT

#include "cs50.h"

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Track allocated strings so we can free them at exit.
static string *strings = NULL;
static size_t allocations = 0;

static void teardown(void)
{
    if (strings != NULL)
    {
        for (size_t i = 0; i < allocations; i++)
        {
            free(strings[i]);
        }
        free(strings);
    }
}

string get_string(const char *format, ...)
{
    // Grow tracking array (register teardown once).
    if (allocations == 0)
    {
        atexit(teardown);
    }

    // Print prompt.
    if (format != NULL)
    {
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }

    // Read a line, growing buffer as needed.
    string buffer = NULL;
    size_t capacity = 0;
    size_t size = 0;
    int c;
    while ((c = fgetc(stdin)) != '\n' && c != EOF)
    {
        // Grow buffer if necessary.
        if (size + 1 > capacity)
        {
            if (capacity < SIZE_MAX)
            {
                capacity = capacity ? capacity * 2 : 16;
            }
            else
            {
                free(buffer);
                return NULL;
            }
            string temp = realloc(buffer, capacity);
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[size++] = (char) c;
    }

    // Return NULL on EOF with no input.
    if (size == 0 && c == EOF)
    {
        free(buffer);
        return NULL;
    }

    // Terminate string.
    string temp = realloc(buffer, size + 1);
    if (temp == NULL)
    {
        free(buffer);
        return NULL;
    }
    buffer = temp;
    buffer[size] = '\0';

    // Track allocation so we can free it at exit.
    string *tmp = realloc(strings, sizeof(string) * (allocations + 1));
    if (tmp == NULL)
    {
        free(buffer);
        return NULL;
    }
    strings = tmp;
    strings[allocations++] = buffer;

    return buffer;
}

char get_char(const char *format, ...)
{
    while (1)
    {
        va_list ap;
        va_start(ap, format);
        // Reprint the prompt via get_string; format applied once.
        char *line = NULL;
        if (format != NULL)
        {
            vprintf(format, ap);
        }
        va_end(ap);

        line = get_string(NULL);
        if (line == NULL)
        {
            return CHAR_MAX;
        }
        if (strlen(line) == 1)
        {
            return line[0];
        }
    }
}

double get_double(const char *format, ...)
{
    while (1)
    {
        va_list ap;
        va_start(ap, format);
        if (format != NULL)
        {
            vprintf(format, ap);
        }
        va_end(ap);

        string line = get_string(NULL);
        if (line == NULL)
        {
            return DBL_MAX;
        }
        if (line[0] != '\0' && !isspace((unsigned char) line[0]))
        {
            char *tail;
            errno = 0;
            double d = strtod(line, &tail);
            if (errno == 0 && *tail == '\0' && d != HUGE_VAL && d != -HUGE_VAL)
            {
                if (strcspn(line, "XxEePpInInAaNn") == strlen(line))
                {
                    return d;
                }
            }
        }
    }
}

float get_float(const char *format, ...)
{
    while (1)
    {
        va_list ap;
        va_start(ap, format);
        if (format != NULL)
        {
            vprintf(format, ap);
        }
        va_end(ap);

        string line = get_string(NULL);
        if (line == NULL)
        {
            return FLT_MAX;
        }
        if (line[0] != '\0' && !isspace((unsigned char) line[0]))
        {
            char *tail;
            errno = 0;
            float f = strtof(line, &tail);
            if (errno == 0 && *tail == '\0' && f != HUGE_VALF && f != -HUGE_VALF)
            {
                if (strcspn(line, "XxEePpInInAaNn") == strlen(line))
                {
                    return f;
                }
            }
        }
    }
}

int get_int(const char *format, ...)
{
    while (1)
    {
        va_list ap;
        va_start(ap, format);
        if (format != NULL)
        {
            vprintf(format, ap);
        }
        va_end(ap);

        string line = get_string(NULL);
        if (line == NULL)
        {
            return INT_MAX;
        }
        if (line[0] != '\0' && !isspace((unsigned char) line[0]))
        {
            char *tail;
            errno = 0;
            long n = strtol(line, &tail, 10);
            if (errno == 0 && *tail == '\0' && n >= INT_MIN && n < INT_MAX)
            {
                return (int) n;
            }
        }
    }
}

int64_t get_long(const char *format, ...)
{
    while (1)
    {
        va_list ap;
        va_start(ap, format);
        if (format != NULL)
        {
            vprintf(format, ap);
        }
        va_end(ap);

        string line = get_string(NULL);
        if (line == NULL)
        {
            return INT64_MAX;
        }
        if (line[0] != '\0' && !isspace((unsigned char) line[0]))
        {
            char *tail;
            errno = 0;
            // Use strtoll for a guaranteed 64-bit parse (Windows `long` is 32-bit).
            long long n = strtoll(line, &tail, 10);
            if (errno == 0 && *tail == '\0' && n < INT64_MAX)
            {
                return (int64_t) n;
            }
        }
    }
}
