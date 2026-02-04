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
tree_node_t* tree_min_node(tree_node_t* root);
tree_node_t* tree_max_node(tree_node_t* root);
void tree_preorder_traversal(tree_node_t* root);
void tree_inorder_traversal(tree_node_t* root);
void tree_postorder_traversal(tree_node_t* root);
tree_node_t* tree_create_node(const char* word);
void tree_free(tree_node_t* root);
void tree_print_visual(tree_node_t* root, int level);
int tree_height(tree_node_t* root);
tree_node_t* tree_delete_by_letter(tree_node_t* root, char letter, int* deleted_count);
int tree_read_from_file(const char* filename, tree_node_t** root);
double tree_measure_deletion_time(tree_node_t** root, char letter);
double file_measure_deletion_time(const char* filename, char letter);
void tree_export_to_dot(tree_node_t* root, const char* output_filename);
tree_node_t* tree_copy(tree_node_t* root);
#endif