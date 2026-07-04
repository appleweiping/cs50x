// CS50x Problem Set 3 — Tideman (ranked pairs / Condorcet method)
//
// Voters rank candidates; from the ballots we build a directed graph of "who beats whom".
// Pairs of candidates where one beats the other are sorted by strength of victory and
// locked into the graph one at a time, skipping any edge that would create a cycle. The
// source of the resulting graph (the candidate with no incoming locked edges) is the
// Condorcet-style winner. The seven TODO functions are implemented in full.

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Maximum number of candidates.
#define MAX 9

// preferences[i][j] = number of voters who prefer candidate i over candidate j.
int preferences[MAX][MAX];

// locked[i][j] = true if there is a locked-in edge pointing from i to j.
bool locked[MAX][MAX];

// Each pair has a winner and a loser candidate index.
typedef struct
{
    int winner;
    int loser;
} pair;

// Candidate names and the array of pairs.
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes.
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
static bool creates_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage.
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates.
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked-in pairs.
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes.
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is the candidate index of the ith preference.
        int ranks[candidate_count];

        // Query for each rank.
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Set ranks[rank] to the index of the candidate named `name`; return success.
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranking: for every pair (i ranked above j),
// increment preferences[ranks[i]][ranks[j]].
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record every pair of candidates where one is strictly preferred over the other.
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            // Ties add no pair.
        }
    }
}

// Sort pairs in decreasing order of strength of victory (margin), via insertion sort.
void sort_pairs(void)
{
    for (int i = 1; i < pair_count; i++)
    {
        pair key = pairs[i];
        int key_strength = preferences[key.winner][key.loser] - preferences[key.loser][key.winner];

        int j = i - 1;
        while (j >= 0)
        {
            int strength = preferences[pairs[j].winner][pairs[j].loser] -
                           preferences[pairs[j].loser][pairs[j].winner];
            if (strength >= key_strength)
            {
                break;
            }
            pairs[j + 1] = pairs[j];
            j--;
        }
        pairs[j + 1] = key;
    }
}

// Lock in each pair in sorted order, skipping any edge that would create a cycle.
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!creates_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// Depth-first check: would locking winner->loser create a cycle? True if `loser` can
// already reach `winner` through the currently locked edges.
static bool creates_cycle(int winner, int loser)
{
    if (loser == winner)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (creates_cycle(winner, i))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the source of the locked graph: the candidate with no incoming locked edges.
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool has_incoming = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                has_incoming = true;
                break;
            }
        }
        if (!has_incoming)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
