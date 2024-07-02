// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // find hash position of word to be compared
    int h = hash(word);

    // filter table by hash position
    node *ptr = table[h];

    // loop through linked list
    while (ptr != NULL)
    {
        // compare list value with word
        if (strcasecmp(ptr->word, word) == 0)
        {
            // end loop and return found (true)
            return true;
            break;
        }
        // moves to next position
        ptr = ptr->next;
    }
    // if reaches out the end of the linked list, returns not found (false)
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }

    // initialize list
    // node *list = NULL;

    // initialize array position
    int h = 0;

    char w[LENGTH + 1];

    // Read each word in the file
    while (fscanf(source, "%s\n", w) != EOF)
    {
        // alocate memory
        node *n = malloc(sizeof(node));
        // check for memory allocation error
        if (n == NULL)
        {
            return false;
        }

        // copy word read into node
        strcpy(n->word, w);

        // check for hash position
        if (hash(w) != h)
        {
            h = hash(w);
        }

        // check if list already has values
        if (table[h] == NULL)
        {
            // if it is the first element, put it in the last position
            n->next = NULL;
        }
        else
        {
            // otherwise put it in the first position
            n->next = table[h];
        }

        // update list
        table[h] = n;
    }

    // Close the dictionary file
    fclose(source);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // initialize counter
    int ct = 0;

    // loop throught hashtable
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            ct += 1;
            ptr = ptr->next;
        }
    }

    return ct;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *tmp;
        node *curr = table[i];
        while (curr != NULL)
        {
            tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }

    return true;
}
