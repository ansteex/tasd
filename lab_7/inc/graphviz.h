#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "tree.h"
#include "avl.h"

void bst_to_dot(tree_node_t* root, const char* filename);
void avl_to_dot(avl_node_t* root, const char* filename);
void compare_trees_to_dot(tree_node_t* bst_root, avl_node_t* avl_root, const char* filename);

#endif