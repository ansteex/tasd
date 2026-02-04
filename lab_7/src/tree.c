#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

tree_node_t* tree_create_node(const char* word) {
    tree_node_t* new_node = malloc(sizeof(tree_node_t));
    if (!new_node) return NULL;
    
    strncpy(new_node->word, word, MAX_WORD_LENGTH - 1);
    new_node->word[MAX_WORD_LENGTH - 1] = '\0';
    new_node->left = NULL;
    new_node->right = NULL;
    
    return new_node;
}

tree_node_t* tree_insert(tree_node_t* root, const char* word) {
    if (root == NULL) {
        return tree_create_node(word);
    }
    
    int cmp = strcmp(word, root->word);
    if (cmp < 0) {
        root->left = tree_insert(root->left, word);
    }
    else if (cmp > 0) {
        root->right = tree_insert(root->right, word);
    }
    
    return root;
}

tree_node_t* tree_search(tree_node_t* root, const char* word) {
    if (root == NULL || strcmp(root->word, word) == 0) {
        return root;
    }
    
    if (strcmp(word, root->word) < 0) {
        return tree_search(root->left, word);
    } else {
        return tree_search(root->right, word);
    }
}
tree_node_t* tree_delete(tree_node_t* root, const char* word) {
    if (root == NULL) return NULL;
    
    int cmp = strcmp(word, root->word);
    
    if (cmp < 0) {
        root->left = tree_delete(root->left, word);
    }
    else if (cmp > 0) {
        root->right = tree_delete(root->right, word);
    }
    else {
        if (root->left == NULL) {
            tree_node_t* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            tree_node_t* temp = root->left;
            free(root);
            return temp;
        }
        else {
            tree_node_t* parent_of_min = root;
            tree_node_t* min_node = root->right;
            while (min_node->left != NULL) {
                parent_of_min = min_node;
                min_node = min_node->left;
            }
            if (parent_of_min != root) {
                parent_of_min->left = min_node->right;
                min_node->right = root->right;
            }
            min_node->left = root->left;
            free(root);
            return min_node;
        }
    }
    return root;
}
tree_node_t* tree_delete_by_letter(tree_node_t* root, char letter, int* deleted_count) {
    if (root == NULL)
        return NULL;
    
    root->left = tree_delete_by_letter(root->left, letter, deleted_count);
    root->right = tree_delete_by_letter(root->right, letter, deleted_count);
    
    if (root->word[0] == letter) {
        (*deleted_count)++;
        return tree_delete(root, root->word);
    }
    
    return root;
}
void tree_print_visual(tree_node_t* root, int level) {
    if (root == NULL) return;
    
    tree_print_visual(root->right, level + 1);
    
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%s\n", root->word);
    
    tree_print_visual(root->left, level + 1);
}

int tree_height(tree_node_t* root) {
    if (root == NULL) return 0;
    
    int left_height = tree_height(root->left);
    int right_height = tree_height(root->right);
    
    return 1 + (left_height > right_height ? left_height : right_height);
}

void tree_free(tree_node_t* root) {
    if (root == NULL) return;
    
    tree_free(root->left);
    tree_free(root->right);
    free(root);
}

int tree_read_from_file(const char* filename, tree_node_t** root) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Ошибка открытия файла: %s\n", filename);
        return ERROR;
    }
    
    char buffer[MAX_LINE_LENGTH];
    char word[MAX_WORD_LENGTH];
    int word_count = 0;
    
    while (fgets(buffer, sizeof(buffer), fp)) {
        char* token = strtok(buffer, WORD_DELIMITERS);
        while (token) {
            if (strlen(token) > 0) {
                strncpy(word, token, MAX_WORD_LENGTH - 1);
                word[MAX_WORD_LENGTH - 1] = '\0';
                
                for (int i = 0; word[i]; i++) {
                    word[i] = tolower(word[i]);
                }
                
                *root = tree_insert(*root, word);
                word_count++;
            }
            token = strtok(NULL, WORD_DELIMITERS);
        }
    }
    
    fclose(fp);
    
    if (word_count == 0) {
        printf("Файл пуст или не содержит слов\n");
        return ERROR;
    }
    
    printf("Загружено %d слов из файла %s\n", word_count, filename);
    return 0;
}

int tree_count_nodes(tree_node_t* root) {
    if (root == NULL) return 0;
    return 1 + tree_count_nodes(root->left) + tree_count_nodes(root->right);
}


void tree_store_inorder(tree_node_t* root, tree_node_t** nodes, int* index) {
    if (root == NULL) return;
    
    tree_store_inorder(root->left, nodes, index);
    nodes[(*index)++] = root;
    tree_store_inorder(root->right, nodes, index);
}


tree_node_t* tree_build_balanced(tree_node_t** nodes, int start, int end) {
    if (start > end) return NULL;
    
    int mid = (start + end) / 2;
    tree_node_t* root = nodes[mid];
    root->left = tree_build_balanced(nodes, start, mid - 1);
    root->right = tree_build_balanced(nodes, mid + 1, end);
    
    return root;
}
tree_node_t* tree_balance(tree_node_t* root) {
    if (root == NULL) return NULL;
    int n = tree_count_nodes(root);
    tree_node_t** nodes = (tree_node_t**)malloc(n * sizeof(tree_node_t*));
    if (!nodes) return root;
    int index = 0;
    tree_store_inorder(root, nodes, &index);
    tree_node_t* balanced_root = tree_build_balanced(nodes, 0, n - 1);
    free(nodes);
    
    return balanced_root;
}