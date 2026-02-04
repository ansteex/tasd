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

tree_node_t* tree_min_node(tree_node_t* root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

tree_node_t* tree_max_node(tree_node_t* root) {
    while (root && root->right != NULL) {
        root = root->right;
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
// tree_node_t* tree_delete(tree_node_t* root, const char* word) {
//     if (root == NULL) return root;
    
//     int cmp = strcmp(word, root->word);
    
//     if (cmp < 0) {
//         root->left = tree_delete(root->left, word);
//     }
//     else if (cmp > 0) {
//         root->right = tree_delete(root->right, word);
//     }
//     else {
//         if (root->left == NULL) {
//             tree_node_t* temp = root->right;
//             free(root);
//             return temp;
//         }
//         else if (root->right == NULL) {
//             tree_node_t* temp = root->left;
//             free(root);
//             return temp;
//         }
        
//         tree_node_t* temp = tree_min_node(root->right);
        
//         strcpy(root->word, temp->word);
        
//         root->right = tree_delete(root->right, temp->word);
//     }//указатели менять
    
//     return root;
// }


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
void tree_preorder_traversal(tree_node_t* root) {
    if (root == NULL) return;
    
    printf("%s ", root->word);
    tree_preorder_traversal(root->left);
    tree_preorder_traversal(root->right);
}

void tree_inorder_traversal(tree_node_t* root) {
    if (root == NULL) return;
    
    tree_inorder_traversal(root->left);
    printf("%s ", root->word);
    tree_inorder_traversal(root->right);
}

void tree_postorder_traversal(tree_node_t* root) {
    if (root == NULL) return;
    
    tree_postorder_traversal(root->left);
    tree_postorder_traversal(root->right);
    printf("%s ", root->word);
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

double tree_measure_deletion_time(tree_node_t** root, char letter) {
    if (*root == NULL) return 0.0;
    int deleted_count = 0;
    clock_t start = clock();
    *root = tree_delete_by_letter(*root, letter,&deleted_count);
    clock_t end = clock();
    
    return (double)(end - start) * MICROSECONDS_IN_SECOND / CLOCKS_PER_SEC;
}
tree_node_t* tree_copy(tree_node_t* root) {
    if (root == NULL) return NULL;
    
    tree_node_t* new_node = tree_create_node(root->word);
    if (new_node) {
        new_node->left = tree_copy(root->left);
        new_node->right = tree_copy(root->right);
    }
    return new_node;
}
double file_measure_deletion_time(const char* filename, char letter) {
    FILE* src = fopen(filename, "r");
    if (!src) return 0.0;
    
    char temp_filename[MAX_FILENAME];
    snprintf(temp_filename, sizeof(temp_filename), "temp_%s", filename);
    FILE* dst = fopen(temp_filename, "w");
    if (!dst) {
        fclose(src);
        return 0.0;
    }
    
    clock_t start = clock();
    
    char buffer[MAX_LINE_LENGTH];
    char word[MAX_WORD_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), src)) {
        char* token = strtok(buffer, WORD_DELIMITERS);
        while (token) {
            if (strlen(token) > 0) {
                strncpy(word, token, MAX_WORD_LENGTH - 1);
                word[MAX_WORD_LENGTH - 1] = '\0';
                
                if (tolower(word[0]) != tolower(letter)) {
                    fprintf(dst, "%s\n", word);
                }
            }
            token = strtok(NULL, WORD_DELIMITERS);
        }
    }
    
    clock_t end = clock();
    
    fclose(src);
    fclose(dst);
    
    remove(filename);
    rename(temp_filename, filename);
    
    return (double)(end - start) * MICROSECONDS_IN_SECOND / CLOCKS_PER_SEC;
}

void tree_generate_dot(tree_node_t* root, FILE* dot_file) {
    if (root == NULL) return;
    
    if (root->left != NULL) {
        fprintf(dot_file, "    \"%s\" -> \"%s\";\n", root->word, root->left->word);
        tree_generate_dot(root->left, dot_file);
    }
    
    if (root->right != NULL) {
        fprintf(dot_file, "    \"%s\" -> \"%s\";\n", root->word, root->right->word);
        tree_generate_dot(root->right, dot_file);
    }
    
    if (root->left == NULL && root->right == NULL) {
        fprintf(dot_file, "    \"%s\";\n", root->word);
    }
}

void tree_export_to_dot(tree_node_t* root, const char* output_filename) {
    if (root == NULL) {
        printf("Дерево пустое, нечего экспортировать\n");
        return;
    }
    
    FILE* dot_file = fopen(output_filename, "w");
    if (!dot_file) {
        printf("Ошибка создания файла %s\n", output_filename);
        return;
    }
    
    fprintf(dot_file, "digraph BST {\n");
    fprintf(dot_file, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=lightblue];\n");
    fprintf(dot_file, "    edge [arrowhead=vee];\n\n");
    
    tree_generate_dot(root, dot_file);
    
    fprintf(dot_file, "}\n");
    fclose(dot_file);
    
    printf("Дерево экспортировано в файл: %s\n", output_filename);
}