# CS50x Problem Set 6 — Mario (more comfortable, Python)
#
# Prints a double half-pyramid of a given height 1..8, with a two-space gap between the
# two half-pyramids. Reprompts until a valid integer height in [1, 8] is entered.


def get_height():
    while True:
        try:
            height = int(input("Height: "))
        except ValueError:
            continue
        if 1 <= height <= 8:
            return height


def main():
    height = get_height()
    for row in range(1, height + 1):
        spaces = " " * (height - row)
        bricks = "#" * row
        print(f"{spaces}{bricks}  {bricks}")


if __name__ == "__main__":
    main()
