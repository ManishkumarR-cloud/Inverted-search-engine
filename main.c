#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

#define TABLE_SIZE 28

// Forward declarations
void validate_args(int argc, char *argv[], filenames_t **file_list);
void create_database(filenames_t *file_list, mainnode_t *hashtable[]);
void update_database(filenames_t *file_list, mainnode_t *hashtable[]);
void display_database(mainnode_t *hashtable[]);
void search_database(mainnode_t *hashtable[], const char *search_word);
void save_database(mainnode_t *hashtable[], const char *out_file);

int main(int argc, char *argv[]) {
    filenames_t *file_list = NULL;             // linked list of valid filenames
    mainnode_t *hashtable[TABLE_SIZE] = {0};   // initialize hash table with NULL
    int choice;
    char word[50];
    char out_file[50];

    // Step 1: Validate command line arguments
    validate_args(argc, argv, &file_list);

    printf("\nValidation successful. Ready to build inverted search database.\n");

    // Menu loop
    while (1) {
        printf("\n========== Inverted Search Menu ==========\n");
        printf("1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Search Word in Database\n");
        printf("4. Update Database\n");
        printf("5. Save Database to File\n");
        printf("6. Exit\n");
        printf("==========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_database(file_list, hashtable);
                printf("\nDatabase created successfully!\n");
                break;

            case 2:
                printf("\nDisplaying Database...\n");
                display_database(hashtable);
                break;

            case 3:
                printf("\nEnter word to search: ");
                scanf("%s", word);
                search_database(hashtable, word);
                break;

            case 4:
                printf("\nUpdating Database with new files...\n");
                update_database(file_list, hashtable);
                printf("\nDatabase updated successfully!\n");
                break;

            case 5:
                printf("\nEnter output filename (e.g., output.txt): ");
                scanf("%s", out_file);
                save_database(hashtable, out_file);
                printf("\nDatabase saved successfully to %s\n", out_file);
                break;

            case 6:
                printf("\nExiting program. Goodbye!\n");
                exit(0);

            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }

    return 0;
}
