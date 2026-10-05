#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structure.h"

#define TABLE_SIZE 28   // 0–25 alphabets, 26 digits, 27 special chars

// Hash function: decide index based on first character
int hash_function(const char *word) {
    char ch = word[0];

    if (isupper(ch)) return ch - 'A';      // 'A' → 0, 'Z' → 25
    else if (islower(ch)) return ch - 'a'; // 'a' → 0, 'z' → 25
    else if (isdigit(ch)) return 26;       // digits → index 26
    else return 27;                        // special chars → index 27
}

// Insert word into hash table
void insert_word(mainnode_t *hashtable[], const char *word, const char *filename) {
    int index = hash_function(word);
    mainnode_t *temp = hashtable[index];

    // Case 1: Bucket empty → create first mainnode + subnode
    if (temp == NULL) {
        mainnode_t *new_main = malloc(sizeof(mainnode_t));
        if (!new_main) {
            printf("Memory allocation failed for mainnode\n");
            return;
        }
        strcpy(new_main->word, word);
        new_main->file_count = 1;
        new_main->link = NULL;

        // Create subnode
        subnode_t *new_sub = malloc(sizeof(subnode_t));
        if (!new_sub) {
            printf("Memory allocation failed for subnode\n");
            free(new_main);
            return;
        }
        new_sub->word_count = 1;
        strcpy(new_sub->f_name, filename);
        new_sub->link = NULL;

        // Link subnode to mainnode
        new_main->sublink = new_sub;

        // Insert mainnode at head of bucket
        hashtable[index] = new_main;
        return;
    }

    // Case 2: Bucket not empty → search for word
    while (temp) {
        if (strcmp(temp->word, word) == 0) {
            // Word found → check subnodes for filename
            subnode_t *sub = temp->sublink;
            while (sub) {
                if (strcmp(sub->f_name, filename) == 0) {
                    sub->word_count++;   // filename already present → increment count
                    return;
                }
                sub = sub->link;
            }
            // Filename not found → create new subnode at head
            subnode_t *new_sub = malloc(sizeof(subnode_t));
            if (!new_sub) {
                printf("Memory allocation failed for subnode\n");
                return;
            }
            new_sub->word_count = 1;
            strcpy(new_sub->f_name, filename);
            new_sub->link = temp->sublink;
            temp->sublink = new_sub;
            temp->file_count++;
            return;
        }
        temp = temp->link;
    }

    // Word not found → create new mainnode + subnode
    mainnode_t *new_main = malloc(sizeof(mainnode_t));
    if (!new_main) {
        printf("Memory allocation failed for mainnode\n");
        return;
    }
    strcpy(new_main->word, word);
    new_main->file_count = 1;

    // Create subnode
    subnode_t *new_sub = malloc(sizeof(subnode_t));
    if (!new_sub) {
        printf("Memory allocation failed for subnode\n");
        free(new_main);
        return;
    }
    new_sub->word_count = 1;
    strcpy(new_sub->f_name, filename);
    new_sub->link = NULL;

    // Link subnode to mainnode
    new_main->sublink = new_sub;

    // Insert new mainnode at head of bucket
    new_main->link = hashtable[index];
    hashtable[index] = new_main;
}
