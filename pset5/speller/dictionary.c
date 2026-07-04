// CS50x Problem Set 5 — Speller dictionary
//
// Implements a spell-checker's dictionary as a hash table of singly-linked-list buckets.
// Words are hashed case-insensitively; check(), load(), size(), and unload() operate on
// that table. The hash function is the widely used djb2 variant, folded to lowercase.

#include "dictionary.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Node in a hash-table bucket.
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in the hash table (a prime for good distribution).
// Must be a macro (not a `const`) so it can size a file-scope array in C.
#define N 65537

// The hash table.
node *table[N];

// Running count of words loaded.
unsigned int word_count = 0;

// Returns true if `word` is in the dictionary, else false (case-insensitive).
bool check(const char *word)
{
    unsigned int index = hash(word);
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes `word` to a bucket index using djb2, lowercasing each character so that the hash
// (and therefore lookups) are case-insensitive.
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    for (const char *p = word; *p != '\0'; p++)
    {
        hash = ((hash << 5) + hash) + (unsigned char) tolower((unsigned char) *p);
    }
    return (unsigned int) (hash % N);
}

// Loads dictionary at `dictionary` into memory; returns true on success.
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(file, "%45s", word) == 1)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }
        strcpy(n->word, word);

        // Insert at the head of the appropriate bucket.
        unsigned int index = hash(word);
        n->next = table[index];
        table[index] = n;

        word_count++;
    }

    fclose(file);
    return true;
}

// Returns the number of words in the dictionary if loaded, else 0.
unsigned int size(void)
{
    return word_count;
}

// Unloads the dictionary from memory; returns true on success.
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        table[i] = NULL;
    }
    return true;
}
