#ifndef HASH_H
#define HASH_H

#include "consts.h"

typedef struct hash_node_chain {
    char word[MAX_WORD_LENGTH];
    struct hash_node_chain* next;
} hash_node_chain_t;

typedef struct {
    hash_node_chain_t** table;
    int size;
    int count;
    int collisions;
} hash_table_chain_t;

typedef struct {
    char word[MAX_WORD_LENGTH];
    int is_occupied;
} hash_node_open_t;

typedef struct {
    hash_node_open_t* table;
    int size;
    int count;
    int collisions;
} hash_table_open_t;
#define MAX_LOAD_FACTOR 0.7
#define MAX_CHAIN_LENGTH 5

void hash_chain_rehash(hash_table_chain_t** ht);
void hash_open_rehash(hash_table_open_t** ht);
int hash_needs_rehash(hash_table_chain_t* ht, int max_comparisons);
int hash_open_needs_rehash(hash_table_open_t* ht, int max_comparisons);
hash_table_chain_t* hash_chain_create(int size);
void hash_chain_insert(hash_table_chain_t* ht, const char* word);
int hash_chain_search(hash_table_chain_t* ht, const char* word, int* comparisons);
void hash_chain_delete(hash_table_chain_t* ht, const char* word);
void hash_chain_delete_by_letter(hash_table_chain_t* ht, char letter, int* deleted_count);
void hash_chain_print(hash_table_chain_t* ht);
void hash_chain_free(hash_table_chain_t* ht);
hash_table_chain_t* hash_chain_read_from_file(const char* filename, int table_size);

hash_table_open_t* hash_open_create(int size);
void hash_open_insert(hash_table_open_t* ht, const char* word);
int hash_open_search(hash_table_open_t* ht, const char* word, int* comparisons);
void hash_open_delete(hash_table_open_t* ht, const char* word);
void hash_open_delete_by_letter(hash_table_open_t* ht, char letter, int* deleted_count);
void hash_open_print(hash_table_open_t* ht);
void hash_open_free(hash_table_open_t* ht);
hash_table_open_t* hash_open_read_from_file(const char* filename, int table_size);

int hash_function(const char* word, int table_size);
void print_hash_statistics(hash_table_chain_t* chain_ht, hash_table_open_t* open_ht);

#endif