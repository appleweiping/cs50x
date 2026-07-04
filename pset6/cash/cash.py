# CS50x Problem Set 6 — Cash (greedy, Python)
#
# Prompts for change owed (in dollars) and prints the minimum number of coins needed to
# make that amount using quarters, dimes, nickels, and pennies (a greedy algorithm).


def get_cents():
    while True:
        try:
            dollars = float(input("Change owed: "))
        except ValueError:
            continue
        if dollars >= 0:
            # Round to the nearest cent to avoid floating-point error.
            return round(dollars * 100)


def main():
    cents = get_cents()
    coins = 0
    for value in (25, 10, 5, 1):
        coins += cents // value
        cents %= value
    print(coins)


if __name__ == "__main__":
    main()
