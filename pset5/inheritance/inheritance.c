// CS50x Problem Set 5 — Inheritance
//
// Simulates the inheritance of blood type alleles over `GENERATIONS` of a family tree.
// The oldest generation's alleles are random; every child inherits one random allele from
// each of its two parents. The program builds the tree, prints it, then frees it.

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles.
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENTATION = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele(void);

int main(void)
{
    // Seed the random number generator.
    srand(time(0));

    // Create a new family with three generations.
    person *p = create_family(GENERATIONS);

    // Print the family tree of blood types.
    print_family(p, 0);

    // Free memory.
    free_family(p);
}

// Recursively create a person and their ancestors up to `generations` deep.
person *create_family(int generations)
{
    // Allocate memory for a new person.
    person *new_person = malloc(sizeof(person));
    if (new_person == NULL)
    {
        return NULL;
    }

    // If there are still generations left to create...
    if (generations > 1)
    {
        // Create two parents by recursion.
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        new_person->parents[0] = parent0;
        new_person->parents[1] = parent1;

        // Inherit one random allele from each parent.
        new_person->alleles[0] = parent0->alleles[rand() % 2];
        new_person->alleles[1] = parent1->alleles[rand() % 2];
    }
    else
    {
        // Base case: oldest generation has no parents and random alleles.
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    return new_person;
}

// Free `p` and all of their ancestors.
void free_family(person *p)
{
    // Handle the base case.
    if (p == NULL)
    {
        return;
    }

    // Free the parents recursively, then the person.
    free_family(p->parents[0]);
    free_family(p->parents[1]);
    free(p);
}

// Print each family member and their alleles, indented by generation.
void print_family(person *p, int generation)
{
    // Handle the base case.
    if (p == NULL)
    {
        return;
    }

    // Print indentation.
    for (int i = 0; i < generation * INDENTATION; i++)
    {
        printf(" ");
    }

    // Print person's generation and blood type.
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n", generation, p->alleles[0],
               p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n", generation, p->alleles[0],
               p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n", generation, p->alleles[0],
               p->alleles[1]);
    }

    // Print the parents of the current person.
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly choose a blood type allele.
char random_allele(void)
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
