// CS50x Problem Set 2 — Caesar
//
// Encrypts a plaintext by rotating each letter forward by a non-negative integer key k,
// preserving case and passing non-alphabetic characters through unchanged. The key is
// supplied as a single command-line argument; usage is validated.

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool only_digits(string s);

int main(int argc, string argv[])
{
    // Require exactly one command-line argument.
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // The key must be a non-negative integer.
    if (!only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int key = atoi(argv[1]);

    string plaintext = get_string("plaintext:  ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char c = plaintext[i];
        if (isupper((unsigned char) c))
        {
            printf("%c", 'A' + (c - 'A' + key) % 26);
        }
        else if (islower((unsigned char) c))
        {
            printf("%c", 'a' + (c - 'a' + key) % 26);
        }
        else
        {
            printf("%c", c);
        }
    }
    printf("\n");
    return 0;
}

// Returns true iff every character of `s` is a decimal digit (and `s` is non-empty).
bool only_digits(string s)
{
    if (s[0] == '\0')
    {
        return false;
    }
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit((unsigned char) s[i]))
        {
            return false;
        }
    }
    return true;
}
