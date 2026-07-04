// CS50x Problem Set 3 — Runoff (ranked-choice / instant-runoff voting)
//
// Voters rank candidates in order of preference. In each round the candidate with a
// majority of first preferences wins; otherwise the last-place candidate is eliminated
// and their voters' next preferences count instead, until a winner emerges (or a tie of
// all remaining candidates). The five TODO functions are implemented in full.

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Maximum number of candidates and voters.
#define MAX_CANDIDATES 9
#define MAX_VOTERS 100

// preferences[i][j] is the jth-preference candidate index for voter i.
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have a name, vote total, and whether they are eliminated.
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates.
int voter_count;
int candidate_count;

// Function prototypes.
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage.
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates.
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes.
    for (int i = 0; i < voter_count; i++)
    {
        // Query for each rank.
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it is invalid.
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
        printf("\n");
    }

    // Keep holding runoffs until winner exists.
    while (true)
    {
        // Calculate votes given remaining candidates.
        tabulate();

        // Check if election has been won.
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidate(s).
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins.
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes.
        eliminate(min);

        // Reset vote counts back to zero.
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference `rank` for `voter` if candidate `name` is valid; return success.
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates: each voter's vote goes to their
// highest-ranked candidate who has not yet been eliminated.
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int cand = preferences[i][j];
            if (!candidates[cand].eliminated)
            {
                candidates[cand].votes++;
                break;
            }
        }
    }
}

// If any candidate has more than half the votes, print their name and return true.
bool print_winner(void)
{
    int majority = voter_count / 2;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > majority)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum vote total among non-eliminated candidates.
int find_min(void)
{
    int min = voter_count + 1;
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes < min)
        {
            min = candidates[i].votes;
        }
    }
    return min;
}

// Return true if every remaining (non-eliminated) candidate has `min` votes (a tie).
bool is_tie(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            return false;
        }
    }
    return true;
}

// Eliminate every non-eliminated candidate whose vote total equals `min`.
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}
