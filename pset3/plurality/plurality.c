// CS50x Problem Set 3 — Plurality
//
// Runs a plurality election: each voter casts one vote for a candidate by name; the
// candidate(s) with the most votes win. Candidates are given as command-line arguments.
// (The vote()/print_winner() functions are the parts CS50 leaves as TODOs.)

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Maximum number of candidates.
#define MAX 9

// Candidates have a name and a vote count.
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates.
candidate candidates[MAX];

// Number of candidates.
int candidate_count;

// Function prototypes.
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage.
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate the array of candidates.
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters.
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Record the vote, if valid.
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    print_winner();
}

// Update vote totals given a new vote. Returns true if `name` matches a candidate.
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner(s) of the election (there may be a tie for the most votes).
void print_winner(void)
{
    // Find the maximum vote total.
    int max_votes = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > max_votes)
        {
            max_votes = candidates[i].votes;
        }
    }

    // Print every candidate with that many votes.
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == max_votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
}
