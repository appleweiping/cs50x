#!/bin/sh
# Download the CS50 dictionaries used by pset5/speller.
#
# The `large` dictionary (143,091 words) is not committed to this repo (it is a large data
# file belonging to the CS50 course). Run this script once to fetch it locally; it is
# git-ignored. The `small` dictionary and sample texts are committed for quick tests.

set -e
DIR="$(cd "$(dirname "$0")" && pwd)"
BASE="https://raw.githubusercontent.com/AhmedKhaledGamil/CS50/master/pset5/speller/dictionaries"

echo "Downloading large dictionary (143,091 words) ..."
curl -fsSL "$BASE/large" -o "$DIR/large"

lines=$(wc -l < "$DIR/large")
echo "Done: $DIR/large ($lines words)"
