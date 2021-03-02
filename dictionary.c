// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

//custom (personal) function prototype
bool add_word(char *word);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// N is a prime number to minimize collisions
const unsigned int N = 143093;

// Number of words in dictionary;
unsigned int dict_words = 0;

//global variable to keep track of whether dictionary is loaded
bool dict_loaded = false;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Copy word to new string tempword to allow for use of tolower()
    int wordlen = strlen(word);
    char *tempword = malloc(LENGTH * sizeof(char));
    strncpy(tempword, word, LENGTH);

    //Loop through tempword, changing each character to lowercase
    char c;
    for (int i = 0; i < wordlen + 1; i++)
    {
        c = tempword[i];
        tempword[i] = tolower(c);
    }

    //Hash the word to be checked against dictionary
    unsigned int hashed = hash(tempword);

    //If index in table is null, word is not in dictionary
    if (table[hashed] == NULL)
    {
        free(tempword);
        return false;
    }
    //Check if word is in the first node of the hash table
    else if (strcmp(tempword, table[hashed]->word) == 0)
    {
        free(tempword);
        return true;
    }
    //If linked list exists & word isn't in first node, loop through subsequent nodes, checking for word match
    else
    {
        for (node *temp = table[hashed]; temp != NULL; temp = temp->next)
        {
            if (strcmp(tempword, temp->word) == 0)
            {
                free(tempword);
                return true;
            }
        }
        free(tempword);
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long result;

    //ATTRIBUTION: this function uses the 'djb2' hash function, originally reported by Dan Bernstein (found at http://www.cse.yorku.ca/~oz/hash.html)
    unsigned long hash = 5381;
    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    hash = hash % N;
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    //Open dictionary file to file stream
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    //Declare iteration variables for loading words from dictionary file into memory
    int index = 0;
    char word[LENGTH + 1];
    char c;

    while (true)
    {
        //Load dictionary into memory, one character at a time
        int out = fread(&c, sizeof(char), 1, file);

        //If EOF reached while not finished loading current word into memory, finish loading current word to memory & return true
        if (out != 1 && index > 0)
        {
            word[index] = '\0';
            dict_words++;
            if (add_word(word) == false)
            {
                printf("failed to add word %s to dictionary\n", word);
                fclose(file);
                return false;
            }
            dict_loaded = true;
            fclose(file);
            return true;
        }
        //If EOF reached and there is no current word still to be loaded into memory, return true
        else if (out != 1 && index == 0)
        {
            dict_loaded = true;
            fclose(file);
            return true;
        }
        //If in the middle of word to be loaded into memory, continue loading word into memory
        else if (c != '\n')
        {
            word[index] = c;
            index++;
        }
        //Must be at the end of a word; add string terminator and call add_word
        else
        {
            word[index] = '\0';
            /*if (dict_words % 1000 == 0)
            {
                printf("current word: %s, count: %i (index: %i)\n", word, dict_words, index);
            }*/
            dict_words++;
            if (add_word(word) == false)
            {
                printf("failed to add word %s to dictionary\n", word);
                fclose(file);
                return false;
            }
            index = 0;
        }
    }
    fclose(file);
    return false;
}

//Loads word into memory, creating linked list or adding to beginning of existing list as necessary
bool add_word(char *word)
{
    //Hash the word to be added
    unsigned int hashed = hash(word);

    //Allocate memory for new node, copy word to the new node
    node *n = malloc(sizeof(node));
    strncpy(n->word, word, LENGTH);

    //Add node to index in hash table, updating previously first node if necessary
    if (table[hashed] == NULL)
    {
        n->next = NULL;
        table[hashed] = n;
        return true;
    }
    else
    {
        n->next = table[hashed];
        table[hashed] = n;
        return true;
    }

    printf("error adding word '%s' to dictionary\n", word);
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (dict_loaded == false)
    {
        return 0;
    }
    else
    {
        return dict_words;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //Declare error variable for tracking success
    bool error = false;

    //Iterate along entire hash table
    for (int i = 0; i <= N; i++)
    {
        //If single node exists at current index of hash table, free that node
        if (table[i] != NULL && table[i]->next == NULL)
        {
            //printf("freeing '%s' (no collisions)\n", table[i]->word);
            free(table[i]);
            table[i] = NULL;
        }
        //If two or more nodes exist at current index, iterate through linked list, freeing nodes one at a time
        else if (table[i] != NULL && table[i]->next != NULL)
        {
            while (table[i] != NULL)
            {
                //printf("freeing '%s' (next node: %p)", table[i]->word, table[i]->next);
                node *temp = table[i]->next;
                free(table[i]);
                table[i] = temp;
            }
            table[i] = NULL;
        }
    }

    //Iterate through entire hash table once again, checking that every pointer in table points to NULL
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            error = true;
        }
    }
    if (error == false)
    {
        return true;
    }
    else
    {
        return false;
    }
}
