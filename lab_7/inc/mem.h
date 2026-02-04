#ifndef MEM_H
#define MEM_H

#include "tree.h"
#include "avl.h"
#include "hash.h"

typedef struct {
    size_t bst_bytes;
    size_t avl_bytes;
    size_t chain_hash_bytes;
    size_t open_hash_bytes;
    int bst_nodes;
    int avl_nodes;
    int chain_hash_items;
    int open_hash_items;
} memory_stats_t;


size_t calculate_bst_memory(tree_node_t* root);
size_t calculate_avl_memory(avl_node_t* root);
size_t calculate_hash_chain_memory(hash_table_chain_t* ht);
size_t calculate_hash_open_memory(hash_table_open_t* ht);


memory_stats_t get_memory_statistics(tree_node_t* bst_root, avl_node_t* avl_root,
                                     hash_table_chain_t* chain_ht, hash_table_open_t* open_ht);


void print_memory_statistics(memory_stats_t stats);
void print_detailed_memory_comparison(tree_node_t* bst_root, avl_node_t* avl_root,
                                      hash_table_chain_t* chain_ht, hash_table_open_t* open_ht);

#endif