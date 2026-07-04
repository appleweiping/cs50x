# CS50x Problem Set 6 — DNA
#
# Identifies a person from a DNA sequence by counting the longest run of consecutive
# repeats of each Short Tandem Repeat (STR) and matching those counts against a CSV
# database of people. Usage: python dna.py databases/small.csv sequences/1.txt

import csv
import sys


def longest_match(sequence: str, subsequence: str) -> int:
    """Return the length of the longest run of back-to-back `subsequence` in `sequence`."""
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Try starting the run at each position in the sequence.
    for i in range(sequence_length):
        count = 0
        # Count consecutive, non-overlapping repeats starting at i.
        while True:
            start = i + count * subsequence_length
            end = start + subsequence_length
            if sequence[start:end] == subsequence:
                count += 1
            else:
                break
        longest_run = max(longest_run, count)

    return longest_run


def main():
    # Require exactly two command-line arguments.
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    database_path, sequence_path = sys.argv[1], sys.argv[2]

    # Read the database into a list of dict rows; the first column is the person's name.
    with open(database_path, newline="") as f:
        reader = csv.DictReader(f)
        strs = [field for field in reader.fieldnames if field != "name"]
        people = list(reader)

    # Read the DNA sequence.
    with open(sequence_path) as f:
        sequence = f.read().strip()

    # Compute the longest run of each STR in the sequence.
    counts = {s: longest_match(sequence, s) for s in strs}

    # A person matches if every STR count equals theirs.
    for person in people:
        if all(int(person[s]) == counts[s] for s in strs):
            print(person["name"])
            return

    print("No match")


if __name__ == "__main__":
    main()
