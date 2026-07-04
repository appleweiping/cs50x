#!/bin/sh
# Download the CS50 movies.db (IMDb data) used by pset7/movies.
#
# movies.db is ~82 MB and is NOT committed to this repo (it is large course data).
# Run this once to fetch the official distribution and place movies.db beside the queries.

set -e
DIR="$(cd "$(dirname "$0")" && pwd)"
TMP="$(mktemp -d)"

echo "Downloading movies.zip (~49 MB) from cs50.net ..."
curl -fSL "https://cdn.cs50.net/2023/fall/psets/7/movies.zip" -o "$TMP/movies.zip"

echo "Extracting movies.db ..."
unzip -o -q "$TMP/movies.zip" -d "$TMP"
cp "$TMP/movies/movies.db" "$DIR/movies.db"
rm -rf "$TMP"

echo "Done: $DIR/movies.db"
