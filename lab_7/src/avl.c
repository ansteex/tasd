#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

avl_node_t* avl_create_node(const char* word) {
    avl_node_t* node = malloc(sizeof(avl_node_t));
    if (!node) return NULL;
    
    strncpy(node->word, word, MAX_WORD_LENGTH - 1);
    node->word[MAX_WORD_LENGTH - 1] = '\0';
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

int avl_height(avl_node_t* node) {
    return node ? node->height : 0;
}

int avl_balance_factor(avl_node_t* node) {
    return node ? avl_height(node->left) - avl_height(node->right) : 0;
}

void avl_update_height(avl_node_t* node) {
    if (node) {
        int left_height = avl_height(node->left);
        int right_height = avl_height(node->right);
        node->height = (left_height > right_height ? left_height : right_height) + 1;
    }
}

avl_node_t* avl_rotate_right(avl_node_t* y) {
    avl_node_t* x = y->left;
    avl_node_t* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    avl_update_height(y);
    avl_update_height(x);
    
    return x;
}

avl_node_t* avl_rotate_left(avl_node_t* x) {
    avl_node_t* y = x->right;
    avl_node_t* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    avl_update_height(x);
    avl_update_height(y);
    
    return y;
}

avl_node_t* avl_balance(avl_node_t* node) {
    if (!node) return NULL;
    
    avl_update_height(node);
    int balance = avl_balance_factor(node);
    
    if (balance > 1 && avl_balance_factor(node->left) >= 0)
        return avl_rotate_right(node);
    
    if (balance < -1 && avl_balance_factor(node->right) <= 0)
        return avl_rotate_left(node);
    
    if (balance > 1 && avl_balance_factor(node->left) < 0) {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }
    
    if (balance < -1 && avl_balance_factor(node->right) > 0) {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }
    
    return node;
}

avl_node_t* avl_insert(avl_node_t* root, const char* word) {
    if (!root) return avl_create_node(word);
    
    int cmp = strcmp(word, root->word);
    
    if (cmp < 0) {
        root->left = avl_insert(root->left, word);
    } else if (cmp > 0) {
        root->right = avl_insert(root->right, word);
    } else {
        return root;
    }
    
    return avl_balance(root);
}

avl_node_t* avl_min_node(avl_node_t* root) {
    while (root && root->left)
        root = root->left;
    return root;
}

avl_node_t* avl_delete(avl_node_t* root, const char* word) {
    if (!root) return NULL;
    
    int cmp = strcmp(word, root->word);
    
    if (cmp < 0) {
        root->left = avl_delete(root->left, word);
    } else if (cmp > 0) {
        root->right = avl_delete(root->right, word);
    } else {
        if (!root->left || !root->right) {
            avl_node_t* temp = root->left ? root->left : root->right;
            
            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            
            free(temp);
        } else {
            avl_node_t* temp = avl_min_node(root->right);
            strcpy(root->word, temp->word);
            root->right = avl_delete(root->right, temp->word);
        }
    }
    
    if (!root) return NULL;
    
    return avl_balance(root);
}

avl_node_t* avl_search(avl_node_t* root, const char* word) {
    if (!root || strcmp(root->word, word) == 0)
        return root;
    
    if (strcmp(word, root->word) < 0)
        return avl_search(root->left, word);
    else
        return avl_search(root->right, word);
}

avl_node_t* avl_delete_by_letter(avl_node_t* root, char letter, int* deleted_count) {
    if (!root) return NULL;
    
    root->left = avl_delete_by_letter(root->left, letter, deleted_count);
    root->right = avl_delete_by_letter(root->right, letter, deleted_count);
    
    if (root->word[0] == letter) {
        (*deleted_count)++;
        return avl_delete(root, root->word);
    }
    
    return root;
}

int avl_count_comparisons(avl_node_t* root, const char* word) {
    int comparisons = 0;
    avl_node_t* current = root;
    
    while (current) {
        comparisons++;
        int cmp = strcmp(word, current->word);
        
        if (cmp == 0) break;
        else if (cmp < 0) current = current->left;
        else current = current->right;
    }
    
    return comparisons;
}

avl_node_t* avl_read_from_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Ошибка открытия файла: %s\n", filename);
        return NULL;
    }
    
    avl_node_t* root = NULL;
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
                
                root = avl_insert(root, word);
                word_count++;
            }
            token = strtok(NULL, WORD_DELIMITERS);
        }
    }
    
    fclose(fp);
    
    if (word_count == 0) {
        printf("Файл пуст или не содержит слов\n");
        return NULL;
    }
    
    printf("Загружено %d слов в AVL-дерево из файла %s\n", word_count, filename);
    return root;
}

void avl_preorder_traversal(avl_node_t* root) {
    if (!root) return;
    
    printf("%s ", root->word);
    avl_preorder_traversal(root->left);
    avl_preorder_traversal(root->right);
}

void avl_inorder_traversal(avl_node_t* root) {
    if (!root) return;
    
    avl_inorder_traversal(root->left);
    printf("%s ", root->word);
    avl_inorder_traversal(root->right);
}

void avl_postorder_traversal(avl_node_t* root) {
    if (!root) return;
    
    avl_postorder_traversal(root->left);
    avl_postorder_traversal(root->right);
    printf("%s ", root->word);
}

void avl_free(avl_node_t* root) {
    if (!root) return;
    
    avl_free(root->left);
    avl_free(root->right);
    free(root);
}

void avl_print_visual(avl_node_t* root, int level) {
    if (!root) return;
    
    avl_print_visual(root->right, level + 1);
    
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%s (h:%d, b:%d)\n", root->word, root->height, 
           avl_balance_factor(root));
    
    avl_print_visual(root->left, level + 1);
}