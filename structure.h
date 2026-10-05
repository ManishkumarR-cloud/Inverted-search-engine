#ifndef STRUCTURE_H
#define STRUCTURE_H

#define FNAME_SIZE 50
#define WORD_SIZE 50

// Subnode: represents occurrence of a word in a file
typedef struct sub {
    int word_count;              // how many times the word appears in this file
    char f_name[FNAME_SIZE];     // file name
    struct sub *link;            // pointer to next subnode
} subnode_t;

// Filenames linked list: stores all valid filenames
typedef struct file {
    char filename[WORD_SIZE];    // file name string
    struct file *link;           // pointer to next filename node
} filenames_t;

// Main node: represents a unique word in the database
typedef struct mainnode {
    int file_count;              // number of files containing this word
    char word[WORD_SIZE];        // the word itself
    struct mainnode *link;       // pointer to next mainnode (chaining in hash table)
    struct sub *sublink;         // head pointer to subnode list (files where word occurs)
} mainnode_t;

#endif
