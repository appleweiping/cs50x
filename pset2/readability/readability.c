// CS50x Problem Set 2 — Readability
//
// Computes the Coleman-Liau index of a passage of text and prints the U.S. grade level
// required to understand it ("Before Grade 1", "Grade N", or "Grade 16+").

#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string text = get_string("Text: ");

    int letters = 0;
    int words = 1; // number of words = (spaces + 1) for non-empty text
    int sentences = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];
        if (isalpha((unsigned char) c))
        {
            letters++;
        }
        else if (c == ' ')
        {
            words++;
        }
        else if (c == '.' || c == '!' || c == '?')
        {
            sentences++;
        }
    }

    // Coleman-Liau: index = 0.0588 * L - 0.296 * S - 15.8
    // where L = avg letters per 100 words, S = avg sentences per 100 words.
    float L = (float) letters / words * 100;
    float S = (float) sentences / words * 100;
    int index = (int) round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}
