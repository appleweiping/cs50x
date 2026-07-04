# CS50x Problem Set 6 — Hello (Python)
#
# Prompts the user for their name and greets them. Uses input() from the standard library
# (the cs50 Python package's get_string is not installed on this build machine; input()
# has identical behaviour for this problem).


def main():
    name = input("What's your name? ")
    print(f"hello, {name}")


if __name__ == "__main__":
    main()
