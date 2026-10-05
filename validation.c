#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structure.h"

// Function to check if file has .txt extension
int check_txt_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');   // find last '.'
    return (dot && strcmp(dot, ".txt") == 0);   // return true if ".txt"
}

// Function to check if file is non-empty
int check_non_empty(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;                          // file not found
    int ch = fgetc(fp);                         // try to read one character
    fclose(fp);
    return (ch != EOF);                         // true if at least one char present
}

// Function to insert filename into linked list if not duplicate
int insert_filename(filenames_t **head, const char *filename) {
    filenames_t *temp = *head;

    // Traverse list to check for duplicates
    while (temp) {
        if (strcmp(temp->filename, filename) == 0) {
            printf("Error: Duplicate file '%s' detected. Random error!\n", filename);
            return 0; // duplicate found
        }
        temp = temp->link;
    }

    // Create new node for filename
    filenames_t *new_node = malloc(sizeof(filenames_t));
    if (!new_node) {
        printf("Memory allocation failed for filename node.\n");
        return 0;
    }
    strcpy(new_node->filename, filename);
    new_node->link = *head;     // insert at head
    *head = new_node;
    return 1;
}

// Main validation function
void validate_args(int argc, char *argv[], filenames_t **file_list) {
    if (argc < 2) {
        printf("Error: No files provided.\n");
        printf("Usage: ./a.out <file1.txt> <file2.txt> ...\n");
        exit(1);
    }

    // Validate each file argument
    for (int i = 1; i < argc; i++) {
        if (!check_txt_extension(argv[i])) {
            printf("Error: File '%s' must have .txt extension.\n", argv[i]);
            exit(1);
        }
        if (!check_non_empty(argv[i])) {
            printf("Error: File '%s' is empty.\n", argv[i]);
            exit(1);
        }
        if (!insert_filename(file_list, argv[i])) {
            exit(1); // duplicate found or memory error
        }
    }
}
