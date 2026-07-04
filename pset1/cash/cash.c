// CS50x Problem Set 1 — Cash (greedy algorithm)
//
// Prompts for an amount of change owed (in dollars) and prints the minimum number of
// coins (quarters, dimes, nickels, pennies) needed to make that change, using a greedy
// algorithm. Reprompts until the user enters a non-negative dollar amount.

#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    // Prompt for change in dollars; reject negatives.
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);

    // Convert to cents, rounding to the nearest integer to avoid float error.
    int cents = round(dollars * 100);

    int coins = 0;

    // Greedily use the largest coin that fits, repeatedly.
    coins += cents / 25;
    cents %= 25;

    coins += cents / 10;
    cents %= 10;

    coins += cents / 5;
    cents %= 5;

    coins += cents; // remaining pennies

    printf("%i\n", coins);
}
