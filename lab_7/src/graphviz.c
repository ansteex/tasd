#include <stdio.h>
#include "graphviz.h"
#include "tree.h"
#include "avl.h"

void bst_print_dot_aux(tree_node_t* node, FILE* stream) {
    if (!node) return;
    
    if (node->left) {
        fprintf(stream, "    \"%s\" -> \"%s\";\n", node->word, node->left->word);
        bst_print_dot_aux(node->left, stream);
    }
    
    if (node->right) {
        fprintf(stream, "    \"%s\" -> \"%s\";\n", node->word, node->right->word);
        bst_print_dot_aux(node->right, stream);
    }
}

void bst_to_dot(tree_node_t* root, const char* filename) {
    if (!root) return;
    
    FILE* f = fopen(filename, "w");
    if (!f) return;
    
    fprintf(f, "digraph BST {\n");
    fprintf(f, "    node [shape=circle];\n");
    bst_print_dot_aux(root, f);
    fprintf(f, "}\n");
    fclose(f);
}

void avl_print_dot_aux(avl_node_t* node, FILE* stream) {
    if (!node) return;
    
    fprintf(stream, "    \"%s_%d\" [label=\"%s\\nh:%d\"];\n", 
            node->word, node->height, node->word, node->height);
    
    if (node->left) {
        fprintf(stream, "    \"%s_%d\" -> \"%s_%d\";\n", 
                node->word, node->height, node->left->word, node->left->height);
        avl_print_dot_aux(node->left, stream);
    }
    
    if (node->right) {
        fprintf(stream, "    \"%s_%d\" -> \"%s_%d\";\n", 
                node->word, node->height, node->right->word, node->right->height);
        avl_print_dot_aux(node->right, stream);
    }
}

void avl_to_dot(avl_node_t* root, const char* filename) {
    if (!root) return;
    
    FILE* f = fopen(filename, "w");
    if (!f) return;
    
    fprintf(f, "digraph AVL {\n");
    fprintf(f, "    node [shape=circle];\n");
    avl_print_dot_aux(root, f);
    fprintf(f, "}\n");
    fclose(f);
}


void print_bst_for_comparison(tree_node_t* node, FILE* f) {
    if (!node) return;
    fprintf(f, "        bst_%p [label=\"%s\"];\n", (void*)node, node->word);
    if (node->left) {
        fprintf(f, "        bst_%p -> bst_%p;\n", (void*)node, (void*)node->left);
        print_bst_for_comparison(node->left, f);
    }
    if (node->right) {
        fprintf(f, "        bst_%p -> bst_%p;\n", (void*)node, (void*)node->right);
        print_bst_for_comparison(node->right, f);
    }
}

void print_avl_for_comparison(avl_node_t* node, FILE* f) {
    if (!node) return;
    fprintf(f, "        avl_%p [label=\"%s\\nh:%d\"];\n", 
            (void*)node, node->word, node->height);
    if (node->left) {
        fprintf(f, "        avl_%p -> avl_%p;\n", (void*)node, (void*)node->left);
        print_avl_for_comparison(node->left, f);
    }
    if (node->right) {
        fprintf(f, "        avl_%p -> avl_%p;\n", (void*)node, (void*)node->right);
        print_avl_for_comparison(node->right, f);
    }
}

void compare_trees_to_dot(tree_node_t* bst_root, avl_node_t* avl_root, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) return;
    
    fprintf(f, "digraph Comparison {\n");
    fprintf(f, "    rankdir=TB;\n");
    fprintf(f, "    node [shape=circle];\n\n");
    
    fprintf(f, "    subgraph cluster_BST {\n");
    fprintf(f, "        label=\"BST\";\n");
    if (bst_root) {
        print_bst_for_comparison(bst_root, f);
    }
    fprintf(f, "    }\n\n");
    
    fprintf(f, "    subgraph cluster_AVL {\n");
    fprintf(f, "        label=\"AVL\";\n");
    if (avl_root) {
        print_avl_for_comparison(avl_root, f);
    }
    fprintf(f, "    }\n");
    fprintf(f, "}\n");
    
    fclose(f);
}