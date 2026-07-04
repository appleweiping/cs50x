# CS50x Problem Set 6 — Credit (Python)
#
# Prompts for a credit-card number and reports whether it is a valid AMEX, MASTERCARD, or
# VISA number (else INVALID) using Luhn's checksum plus length and prefix rules.


def luhn_valid(number: str) -> bool:
    total = 0
    # Process digits from right to left; double every second digit.
    for i, ch in enumerate(reversed(number)):
        digit = int(ch)
        if i % 2 == 1:
            digit *= 2
            if digit > 9:
                digit -= 9
        total += digit
    return total % 10 == 0


def classify(number: str) -> str:
    length = len(number)
    if not luhn_valid(number):
        return "INVALID"
    if length == 15 and number[:2] in ("34", "37"):
        return "AMEX"
    if length == 16 and number[:2] in ("51", "52", "53", "54", "55"):
        return "MASTERCARD"
    if length in (13, 16) and number[0] == "4":
        return "VISA"
    return "INVALID"


def main():
    number = input("Number: ").strip()
    if not number.isdigit():
        print("INVALID")
        return
    print(classify(number))


if __name__ == "__main__":
    main()
