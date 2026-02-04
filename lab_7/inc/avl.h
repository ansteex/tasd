#ifndef AVL_H
#define AVL_H
#include "consts.h"
typedef struct avl_node {
    char word[MAX_WORD_LENGTH];
    int height;
    struct avl_node* left;
    struct avl_node* right;
} avl_node_t;

avl_node_t* avl_create_node(const char* word);
avl_node_t* avl_insert(avl_node_t* root, const char* word);
avl_node_t* avl_delete(avl_node_t* root, const char* word);
avl_node_t* avl_search(avl_node_t* root, const char* word);
avl_node_t* avl_delete_by_letter(avl_node_t* root, char letter, int* deleted_count);

void avl_preorder_traversal(avl_node_t* root);
void avl_inorder_traversal(avl_node_t* root);
void avl_postorder_traversal(avl_node_t* root);

int avl_height(avl_node_t* root);
int avl_balance_factor(avl_node_t* node);
void avl_update_height(avl_node_t* node);
avl_node_t* avl_rotate_right(avl_node_t* y);
avl_node_t* avl_rotate_left(avl_node_t* x);
avl_node_t* avl_balance(avl_node_t* node);

avl_node_t* avl_read_from_file(const char* filename);
void avl_free(avl_node_t* root);
void avl_print_visual(avl_node_t* root, int level);
int avl_count_comparisons(avl_node_t* root, const char* word);

#endif