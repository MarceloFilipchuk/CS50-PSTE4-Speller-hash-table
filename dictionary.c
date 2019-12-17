// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Declares functions to be used at the beginining of the program
bool unload(void);

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Keeps track of the number of words loaded in the dictionary so far according to the "load" function
unsigned long words = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        words++;

        // Copies the word from the dictionary to the 'word' part of the node
        strcpy(new_node->word, word);

        // Links the newly created node to its correspondent place in the hash table
        new_node->next = hashtable[hash(word)];
        hashtable[hash(word)] = new_node;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Creates a pointer to the head of the node
    node *cursor = hashtable[hash(word)];

    // Turns 'word' into all lowercase characters to search for it in the dictionary
    char lowercase_word[strlen(word)+1];
    strcpy(lowercase_word, word);
    for (int l = 0, length = strlen(word); l < length; l++)
    {
        lowercase_word[l] = tolower(lowercase_word[l]);
    }

    // Iterates over the nodes in the correspondent bucket
    while (cursor != NULL)
    {
        if (strcmp(cursor->word, lowercase_word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int k = 0; k < N; k++)
    {
        node *cursor = hashtable[k];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
