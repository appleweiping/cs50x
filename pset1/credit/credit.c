// CS50x Problem Set 1 — Credit
//
// Prompts for a credit card number and reports whether it is a valid AMEX, MASTERCARD,
// or VISA number (or INVALID), using Luhn's checksum plus length/prefix rules.

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // `long long` is 64-bit on every platform (Windows `long` is only 32-bit), matching
    // CS50's 64-bit `long` so a 16-digit card number is held without truncation.
    long long number = get_long("Number: ");

    // Compute Luhn checksum and count total digits.
    long long n = number;
    int digits = 0;
    int sum = 0;

    for (int i = 0; n > 0; i++)
    {
        int digit = n % 10;

        if (i % 2 == 0)
        {
            // Digits in even positions (from the right, 0-indexed) added as-is.
            sum += digit;
        }
        else
        {
            // Odd positions: double, then sum the digits of the product.
            int doubled = digit * 2;
            sum += doubled / 10 + doubled % 10;
        }

        digits++;
        n /= 10;
    }

    // Extract the first two digits (most significant) for prefix checks.
    long long m = number;
    while (m >= 100)
    {
        m /= 10;
    }
    int first_two = (int) m;
    int first = first_two / 10;

    // Valid only if Luhn checksum ends in 0.
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Classify by length + prefix.
    if (digits == 15 && (first_two == 34 || first_two == 37))
    {
        printf("AMEX\n");
    }
    else if (digits == 16 && first_two >= 51 && first_two <= 55)
    {
        printf("MASTERCARD\n");
    }
    else if ((digits == 13 || digits == 16) && first == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
