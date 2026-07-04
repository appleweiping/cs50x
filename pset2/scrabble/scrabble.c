// CS50x Problem Set 2 — Scrabble
//
// Determines which of two words is worth more in Scrabble, scoring each letter by its
// point value (case-insensitive) and ignoring non-alphabetic characters.

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter A-Z.
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3,
                1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Prompt for both players' words.
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score them.
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner (or a tie).
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

// Sum the point values of the alphabetic characters in `word`, case-insensitively.
int compute_score(string word)
{
    int score = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        char c = word[i];
        if (isupper((unsigned char) c))
        {
            score += POINTS[c - 'A'];
        }
        else if (islower((unsigned char) c))
        {
            score += POINTS[c - 'a'];
        }
    }
    return score;
}
