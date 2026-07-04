// CS50x Problem Set 1 — Mario (more comfortable)
//
// Prints a double half-pyramid of a given height 1..8, with a two-space gap between the
// two pyramids, using '#' as bricks. Reprompts until the user enters an int in [1, 8].

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for a height between 1 and 8 (inclusive).
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Print the double half-pyramid.
    for (int row = 0; row < height; row++)
    {
        // Left-aligned spaces so the left pyramid is right-justified.
        for (int spaces = 0; spaces < height - 1 - row; spaces++)
        {
            printf(" ");
        }

        // Left pyramid: row + 1 bricks.
        for (int bricks = 0; bricks <= row; bricks++)
        {
            printf("#");
        }

        // Two-space gap between the pyramids.
        printf("  ");

        // Right pyramid: row + 1 bricks.
        for (int bricks = 0; bricks <= row; bricks++)
        {
            printf("#");
        }

        printf("\n");
    }
}
