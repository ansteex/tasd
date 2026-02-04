#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

size_t calculate_bst_memory(tree_node_t* root) {
    if (root == NULL) return sizeof(tree_node_t*) + sizeof(tree_node_t); 
    
    size_t total = sizeof(tree_node_t); 
    
    
    if (root->left) total += calculate_bst_memory(root->left) - sizeof(tree_node_t*);
    if (root->right) total += calculate_bst_memory(root->right) - sizeof(tree_node_t*);
    
    return total;
}

size_t calculate_avl_memory(avl_node_t* root) {
    if (root == NULL) return sizeof(avl_node_t*) + sizeof(avl_node_t);
    
    size_t total = sizeof(avl_node_t);
    
    if (root->left) total += calculate_avl_memory(root->left) - sizeof(avl_node_t*);
    if (root->right) total += calculate_avl_memory(root->right) - sizeof(avl_node_t*);
    
    return total;
}

size_t calculate_hash_chain_memory(hash_table_chain_t* ht) {
    if (!ht) return 0;
    
    size_t total = sizeof(hash_table_chain_t); 
    total += ht->size * sizeof(hash_node_chain_t*); 
    
    
    for (int i = 0; i < ht->size; i++) {
        hash_node_chain_t* current = ht->table[i];
        while (current) {
            total += sizeof(hash_node_chain_t);
            current = current->next;
        }
    }
    
    return total;
}

size_t calculate_hash_open_memory(hash_table_open_t* ht) {
    if (!ht) return 0;
    
    size_t total = sizeof(hash_table_open_t); 
    total += ht->size * sizeof(hash_node_open_t); 
    
    return total;
}

int count_bst_nodes(tree_node_t* root) {
    if (root == NULL) return 0;
    return 1 + count_bst_nodes(root->left) + count_bst_nodes(root->right);
}

int count_avl_nodes(avl_node_t* root) {
    if (root == NULL) return 0;
    return 1 + count_avl_nodes(root->left) + count_avl_nodes(root->right);
}

memory_stats_t get_memory_statistics(tree_node_t* bst_root, avl_node_t* avl_root,
                                     hash_table_chain_t* chain_ht, hash_table_open_t* open_ht) {
    memory_stats_t stats = {0};
    
    stats.bst_bytes = calculate_bst_memory(bst_root);
    stats.avl_bytes = calculate_avl_memory(avl_root);
    stats.chain_hash_bytes = calculate_hash_chain_memory(chain_ht);
    stats.open_hash_bytes = calculate_hash_open_memory(open_ht);
    
    stats.bst_nodes = count_bst_nodes(bst_root);
    stats.avl_nodes = count_avl_nodes(avl_root);
    stats.chain_hash_items = chain_ht ? chain_ht->count : 0;
    stats.open_hash_items = open_ht ? open_ht->count : 0;
    
    return stats;
}

void print_memory_statistics(memory_stats_t stats) {
    printf("\n=== СТАТИСТИКА ИСПОЛЬЗОВАНИЯ ПАМЯТИ ===\n");
    printf("BST дерево:\n");
    printf("  Узлов: %d\n", stats.bst_nodes);
    printf("  Память: %.2f KB (%.2f MB)\n", 
           stats.bst_bytes / 1024.0, stats.bst_bytes / (1024.0 * 1024.0));
    printf("  Память на узел: %.2f байт\n", 
           stats.bst_nodes > 0 ? (float)stats.bst_bytes / stats.bst_nodes : 0);
    
    printf("\nAVL дерево:\n");
    printf("  Узлов: %d\n", stats.avl_nodes);
    printf("  Память: %.2f KB (%.2f MB)\n", 
           stats.avl_bytes / 1024.0, stats.avl_bytes / (1024.0 * 1024.0));
    printf("  Память на узел: %.2f байт\n", 
           stats.avl_nodes > 0 ? (float)stats.avl_bytes / stats.avl_nodes : 0);
    
    printf("\nХеш-таблица (цепочки):\n");
    printf("  Элементов: %d\n", stats.chain_hash_items);
    printf("  Память: %.2f KB (%.2f MB)\n", 
           stats.chain_hash_bytes / 1024.0, stats.chain_hash_bytes / (1024.0 * 1024.0));
    
    printf("\nХеш-таблица (открытая адресация):\n");
    printf("  Элементов: %d\n", stats.open_hash_items);
    printf("  Память: %.2f KB (%.2f MB)\n", 
           stats.open_hash_bytes / 1024.0, stats.open_hash_bytes / (1024.0 * 1024.0));
    
    printf("\nСравнение эффективности памяти:\n");
    printf("  Эффективность BST: %.2f байт/элемент\n", 
           stats.bst_nodes > 0 ? (float)stats.bst_bytes / stats.bst_nodes : 0);
    printf("  Эффективность AVL: %.2f байт/элемент\n", 
           stats.avl_nodes > 0 ? (float)stats.avl_bytes / stats.avl_nodes : 0);
    printf("  Эффективность цепочек: %.2f байт/элемент\n", 
           stats.chain_hash_items > 0 ? (float)stats.chain_hash_bytes / stats.chain_hash_items : 0);
    printf("  Эффективность открытой: %.2f байт/элемент\n", 
           stats.open_hash_items > 0 ? (float)stats.open_hash_bytes / stats.open_hash_items : 0);
}

void print_detailed_memory_comparison(tree_node_t* bst_root, avl_node_t* avl_root,
                                      hash_table_chain_t* chain_ht, hash_table_open_t* open_ht) {
    memory_stats_t stats = get_memory_statistics(bst_root, avl_root, chain_ht, open_ht);
    print_memory_statistics(stats);
}