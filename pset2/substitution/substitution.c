// CS50x Problem Set 2 — Substitution
//
// Encrypts a plaintext using a substitution cipher whose 26-character key is supplied as
// a single command-line argument. The key must be a permutation of the alphabet (26
// letters, no repeats); case of the plaintext is preserved and non-letters pass through.

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool valid_key(string key);

int main(int argc, string argv[])
{
    // Require exactly one command-line argument (the key).
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];
    if (!valid_key(key))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    string plaintext = get_string("plaintext:  ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char c = plaintext[i];
        if (isupper((unsigned char) c))
        {
            // Map A-Z through the key, preserving uppercase.
            printf("%c", toupper((unsigned char) key[c - 'A']));
        }
        else if (islower((unsigned char) c))
        {
            // Map a-z through the key, preserving lowercase.
            printf("%c", tolower((unsigned char) key[c - 'a']));
        }
        else
        {
            printf("%c", c);
        }
    }
    printf("\n");
    return 0;
}

// A key is valid iff it has exactly 26 characters, all alphabetic, with no letter
// (case-insensitively) repeated.
bool valid_key(string key)
{
    if (strlen(key) != 26)
    {
        return false;
    }

    bool seen[26] = {false};
    for (int i = 0; i < 26; i++)
    {
        char c = key[i];
        if (!isalpha((unsigned char) c))
        {
            return false;
        }
        int idx = tolower((unsigned char) c) - 'a';
        if (seen[idx])
        {
            return false;
        }
        seen[idx] = true;
    }
    return true;
}
