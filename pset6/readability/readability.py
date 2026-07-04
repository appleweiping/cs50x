# CS50x Problem Set 6 — Readability (Python)
#
# Computes the Coleman-Liau index of a passage and prints the U.S. grade level required to
# understand it ("Before Grade 1", "Grade N", or "Grade 16+").


def main():
    text = input("Text: ")

    letters = sum(1 for c in text if c.isalpha())
    words = len(text.split())
    sentences = sum(1 for c in text if c in ".!?")

    # Coleman-Liau: index = 0.0588 * L - 0.296 * S - 15.8
    # where L = avg letters per 100 words, S = avg sentences per 100 words.
    if words == 0:
        grade = 0
    else:
        L = letters / words * 100
        S = sentences / words * 100
        grade = round(0.0588 * L - 0.296 * S - 15.8)

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


if __name__ == "__main__":
    main()
