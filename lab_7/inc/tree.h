#ifndef TREE_H
#define TREE_H
#include <ctype.h>
#include "consts.h"
#include <string.h>
typedef struct tree_node {
    char word[MAX_WORD_LENGTH];
    struct tree_node* left;
    struct tree_node* right;
} tree_node_t;
tree_node_t* tree_insert(tree_node_t* root, const char* word);
tree_node_t* tree_delete(tree_node_t* root, const char* word);
tree_node_t* tree_search(tree_node_t* root, const char* word);
tree_node_t* tree_create_node(const char* word);
void tree_free(tree_node_t* root);
void tree_print_visual(tree_node_t* root, int level);
int tree_height(tree_node_t* root);
tree_node_t* tree_delete_by_letter(tree_node_t* root, char letter, int* deleted_count);
int tree_read_from_file(const char* filename, tree_node_t** root);
tree_node_t* tree_balance(tree_node_t* root);
int tree_count_nodes(tree_node_t* root);
void tree_store_inorder(tree_node_t* root, tree_node_t** nodes, int* index);
tree_node_t* tree_build_balanced(tree_node_t** nodes, int start, int end);
#endif