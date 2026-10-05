#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

#define TABLE_SIZE 28

// External insert_word function from hashtable.c
void insert_word(mainnode_t *hashtable[], const char *word, const char *filename);
int hash_function(const char *word); // external hash function

// Function to read words from files and build database
void create_database(filenames_t *file_list, mainnode_t *hashtable[]) {
    filenames_t *temp_file = file_list;

    // Loop through each file in the linked list
    while (temp_file) {
        FILE *fp = fopen(temp_file->filename, "r");
        if (!fp) {
            printf("Error: Could not open file %s\n", temp_file->filename);
            temp_file = temp_file->link;
            continue;
        }

        char buffer[WORD_SIZE];
        // Read words one by one until EOF
        while (fscanf(fp, "%s", buffer) != EOF) {
            // Insert word into hash table
            insert_word(hashtable, buffer, temp_file->filename);
        }

        fclose(fp);               // close file after processing
        temp_file = temp_file->link; // move to next filename
    }
}

// Function to display the contents of the inverted index (hash table)
void display_database(mainnode_t *hashtable[]) {
    // Print header line aligned with output format
    printf("[Index]   [Word]        [FileCount]   [Filename]       [WordCount]\n");
    printf("-------------------------------------------------------------------\n");

    // Loop through each bucket in the hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        mainnode_t *temp = hashtable[i];   // pointer to traverse mainnode list

        // Traverse all words stored in this bucket
        while (temp) {
            // Print index number, word, and total file count
            printf("[%d]   [%s]   %d file(s) : ", 
                   i, temp->word, temp->file_count);

            // Traverse subnode list (files containing this word)
            subnode_t *sub = temp->sublink;
            while (sub) {
                // Print filename and word occurrence count
                printf("file : %s : %d time(s) ", 
                       sub->f_name, sub->word_count);
                sub = sub->link;   // move to next file node
            }

            printf("\n");  // move to next line after finishing one word entry
            temp = temp->link;    // move to next word in the bucket
        }
    }
}

// Function to search for a specific word in the inverted index
void search_database(mainnode_t *hashtable[], const char *search_word) {
    int index = hash_function(search_word); // compute bucket index for the word
    int found = 0; // flag to track if word is found

    mainnode_t *temp = hashtable[index]; // start at the correct bucket

    // Traverse all words in this bucket
    while (temp) {
        // Compare current word with the search word
        if (strcmp(temp->word, search_word) == 0) {
            found = 1; // mark as found

            // Print header line for clarity
            printf("[Index]   [Word]        [FileCount]   [Filename]       [WordCount]\n");
            printf("-------------------------------------------------------------------\n");

            // Print index, word, and file count
            printf("[%d]   [%s]   %d file(s) : ", 
                   index, temp->word, temp->file_count);

            // Traverse subnode list (files containing this word)
            subnode_t *sub = temp->sublink;
            while (sub) {
                printf("file : %s : %d time(s) ", 
                       sub->f_name, sub->word_count);
                sub = sub->link;
            }
            printf("\n"); // move to next line after finishing one word
            break; // stop once word is found
        }
        temp = temp->link; // move to next word in bucket
    }

    // If word not found in this bucket
    if (!found) {
        printf("Word '%s' not found in database.\n", search_word);
    }
}

// Function to update database with new files
void update_database(filenames_t *file_list, mainnode_t *hashtable[]) {
    // Reuse create_database logic but allow insertion into existing hashtable
    create_database(file_list, hashtable);
}

// Function to save database to a file
void save_database(mainnode_t *hashtable[], const char *out_file) {
    FILE *fp = fopen(out_file, "w");
    if (!fp) {
        printf("Error: Could not open file %s for writing.\n", out_file);
        return;
    }

    fprintf(fp, "[Index]   [Word]        [FileCount]   [Filename]       [WordCount]\n");
    fprintf(fp, "-------------------------------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        mainnode_t *temp = hashtable[i];
        while (temp) {
            fprintf(fp, "[%d]   [%s]   %d file(s) : ", 
                    i, temp->word, temp->file_count);

            subnode_t *sub = temp->sublink;
            while (sub) {
                fprintf(fp, "file : %s : %d time(s) ", 
                        sub->f_name, sub->word_count);
                sub = sub->link;
            }
            fprintf(fp, "\n");
            temp = temp->link;
        }
    }

    fclose(fp);
}
