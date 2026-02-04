#include "tree.h"
#include "avl.h"
#include "hash.h"
#include "graphviz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "mem.h"
tree_node_t* bst_root = NULL;
avl_node_t* avl_root = NULL;
hash_table_chain_t* chain_ht = NULL;
hash_table_open_t* open_ht = NULL;

void clean_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void save_words_to_file(tree_node_t* root, FILE* f) {
    if (!root) return;
    fprintf(f, "%s\n", root->word);
    save_words_to_file(root->left, f);
    save_words_to_file(root->right, f);
}

void collect_words_for_test(tree_node_t* root, char*** words_array, int* index, int* capacity) {
    if (!root) return;
    
    
    if (*index >= *capacity) {
        *capacity *= 2;
        *words_array = realloc(*words_array, *capacity * sizeof(char*));
        if (!*words_array) {
            printf("Ошибка перевыделения памяти\n");
            return;
        }
    }
    
    (*words_array)[(*index)++] = root->word;
    collect_words_for_test(root->left, words_array, index, capacity);
    collect_words_for_test(root->right, words_array, index, capacity);
}

void process_file_for_hash_tables(FILE* fp, hash_table_chain_t* chain_ht, hash_table_open_t* open_ht) {
    char buffer[MAX_LINE_LENGTH];
    char word[MAX_WORD_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), fp)) {
        char* token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}");
        while (token) {
            if (strlen(token) > 0) {
                strncpy(word, token, MAX_WORD_LENGTH - 1);
                word[MAX_WORD_LENGTH - 1] = '\0';
                
                for (int i = 0; word[i]; i++) {
                    word[i] = tolower(word[i]);
                }
                
                hash_chain_insert(chain_ht, word);
                hash_open_insert(open_ht, word);
            }
            token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
        }
    }
}
void test_different_table_sizes(void) {
    if (!bst_root) {
        printf("Сначала загрузите данные\n");
        return;
    }
    
    char filename[100];
    printf("Имя файла (оставьте пустым для текущих данных): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    
    
    if (strlen(filename) > 0) {
        tree_free(bst_root);
        avl_free(avl_root);
        
        bst_root = NULL;
        avl_root = NULL;
        
        if (tree_read_from_file(filename, &bst_root) != 0) {
            printf("Ошибка загрузки данных из файла %s\n", filename);
            return;
        }
    }
    
    
    FILE* temp_file = fopen("temp_test_data.txt", "w");
    if (!temp_file) {
        printf("Ошибка создания временного файла\n");
        return;
    }
    
    save_words_to_file(bst_root, temp_file);
    fclose(temp_file);
    
    int sizes[] = {10, 50, 100, 200, 500, 1000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("\n ТЕСТИРОВАНИЕ РАЗНЫХ РАЗМЕРОВ ТАБЛИЦ \n");
    printf("Размер | Коллизии(цеп) | Коллизии(откр) | Ср.время поиска(цеп) | Ср.время поиска(откр)\n");
    printf("-------------------------------------------------------------------------------------\n");
    
    
    int word_count = tree_count_nodes(bst_root);
    if (word_count == 0) {
        printf("Нет слов для тестирования\n");
        remove("temp_test_data.txt");
        return;
    }
    
    char** words = malloc(word_count * sizeof(char*));
    if (!words) {
        printf("Ошибка выделения памяти\n");
        remove("temp_test_data.txt");
        return;
    }
    
    int index = 0;
    int capacity = word_count;
    collect_words_for_test(bst_root, &words, &index, &capacity);
    
    for (int i = 0; i < num_sizes; i++) {
        int table_size = sizes[i];
        
        
        hash_table_chain_t* test_chain_ht = hash_chain_read_from_file("temp_test_data.txt", table_size);
        hash_table_open_t* test_open_ht = hash_open_read_from_file("temp_test_data.txt", table_size);
        
        if (!test_chain_ht || !test_open_ht) {
            printf("Ошибка создания таблиц размера %d\n", table_size);
            if (test_chain_ht) hash_chain_free(test_chain_ht);
            if (test_open_ht) hash_open_free(test_open_ht);
            continue;
        }
        
        
        clock_t start, end;
        double chain_time_total = 0, open_time_total = 0;
        int chain_comparisons_total = 0, open_comparisons_total = 0;
        int num_tests = 50;
        
        srand(time(NULL));
        
        for (int j = 0; j < num_tests; j++) {
            int random_index = rand() % word_count;
            char* test_word = words[random_index];
            
            int comp;
            
            
            start = clock();
            hash_chain_search(test_chain_ht, test_word, &comp);
            end = clock();
            chain_time_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
            chain_comparisons_total += comp;
            
            
            start = clock();
            hash_open_search(test_open_ht, test_word, &comp);
            end = clock();
            open_time_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
            open_comparisons_total += comp;
        }
        
        
        printf("%6d | %14d | %15d | %21.2f | %21.2f\n",
               table_size,
               test_chain_ht->collisions,
               test_open_ht->collisions,
               chain_time_total / num_tests,
               open_time_total / num_tests);
        
        hash_chain_free(test_chain_ht);
        hash_open_free(test_open_ht);
    }
    
    free(words);
    remove("temp_test_data.txt");
}
void analyze_collision_efficiency(void) {
    if (!bst_root) {
        printf("Сначала загрузите данные\n");
        return;
    }
    
    printf("\n АНАЛИЗ ЭФФЕКТИВНОСТИ ПРИ РАЗНЫХ УРОВНЯХ КОЛЛИЗИЙ \n");
    int sizes[] = {10, 20, 50, 100, 200};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        
        
        hash_table_chain_t* temp_chain = hash_chain_create(size);
        hash_table_open_t* temp_open = hash_open_create(size);
        
        if (!temp_chain || !temp_open) continue;
        
        
        
        
        
        float load_factor = (float)tree_count_nodes(bst_root) / size;
        
        printf("\nРазмер таблицы: %d (коэфф. загрузки: %.2f)\n", size, load_factor);
        printf("Ожидаемые коллизии: %s\n", 
               load_factor > 0.7 ? "высокие" : load_factor > 0.5 ? "средние" : "низкие");
        
        
        clock_t start, end;
        double chain_time = 0, open_time = 0;
        int tests = 50;
        
        for (int j = 0; j < tests; j++) {
            
            char test_word[MAX_WORD_LENGTH];
            sprintf(test_word, "test%d", rand() % 1000);
            
            int comp;
            start = clock();
            hash_chain_search(temp_chain, test_word, &comp);
            end = clock();
            chain_time += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
            
            start = clock();
            hash_open_search(temp_open, test_word, &comp);
            end = clock();
            open_time += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        }
        
        printf("Среднее время поиска:\n");
        printf("  Цепочки: %.2f мкс\n", chain_time / tests);
        printf("  Открытая адресация: %.2f мкс\n", open_time / tests);
        
        hash_chain_free(temp_chain);
        hash_open_free(temp_open);
    }
}
void print_bst_for_dot(tree_node_t* node, FILE* f) {
    if (!node) return;
    fprintf(f, "        bst_%p [label=\"%s\"];\n", (void*)node, node->word);
    if (node->left) {
        fprintf(f, "        bst_%p -> bst_%p;\n", (void*)node, (void*)node->left);
        print_bst_for_dot(node->left, f);
    }
    if (node->right) {
        fprintf(f, "        bst_%p -> bst_%p;\n", (void*)node, (void*)node->right);
        print_bst_for_dot(node->right, f);
    }
}

void print_avl_for_dot(avl_node_t* node, FILE* f) {
    if (!node) return;
    fprintf(f, "        avl_%p [label=\"%s\\nh:%d\"];\n", 
            (void*)node, node->word, node->height);
    if (node->left) {
        fprintf(f, "        avl_%p -> avl_%p;\n", (void*)node, (void*)node->left);
        print_avl_for_dot(node->left, f);
    }
    if (node->right) {
        fprintf(f, "        avl_%p -> avl_%p;\n", (void*)node, (void*)node->right);
        print_avl_for_dot(node->right, f);
    }
}

void load_data(void) {
    char filename[100];
    printf("Файл: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    
    int table_size;
    printf("Размер хеш-таблицы: ");
    if(scanf("%d", &table_size)!=1 || table_size <=0)
    {
        printf("Ошибка ввода размера хеш-таблицы");
        return;
    }
    clean_input_buffer();
    
    tree_free(bst_root);
    avl_free(avl_root);
    if (chain_ht) hash_chain_free(chain_ht);
    if (open_ht) hash_open_free(open_ht);
    
    bst_root = NULL;
    avl_root = NULL;
    
    if (tree_read_from_file(filename, &bst_root) != 0) return;
    
    FILE* fp = fopen(filename, "r");
    if (!fp) return;
    
    char buffer[256];
    char word[100];
    
    while (fgets(buffer, sizeof(buffer), fp)) {
        char* token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}");
        while (token) {
            if (strlen(token) > 0) {
                strncpy(word, token, 99);
                word[99] = '\0';
                
                for (int i = 0; word[i]; i++) {
                    word[i] = tolower(word[i]);
                }
                
                avl_root = avl_insert(avl_root, word);
            }
            token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
        }
    }
    
    fclose(fp);
    
    chain_ht = hash_chain_create(table_size);
    open_ht = hash_open_create(table_size);
    
    fp = fopen(filename, "r");
    if (fp) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            char* token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}");
            while (token) {
                if (strlen(token) > 0) {
                    strncpy(word, token, 99);
                    word[99] = '\0';
                    
                    for (int i = 0; word[i]; i++) {
                        word[i] = tolower(word[i]);
                    }
                    
                    hash_chain_insert(chain_ht, word);
                    hash_open_insert(open_ht, word);
                }
                token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
            }
        }
        fclose(fp);
    }
    
    printf("Загружено\n");
}

void search_word(void) {
    if (!bst_root) {
        printf("Данные не загружены\n");
        return;
    }
    
    char word[100];
    printf("Слово: ");
    fgets(word, sizeof(word), stdin);
    word[strcspn(word, "\n")] = 0;
    
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
    
    clock_t start, end;
    double bst_time, avl_time, chain_time, open_time;
    int chain_comparisons, open_comparisons;
    
    start = clock();
    tree_node_t* bst_result = tree_search(bst_root, word);
    end = clock();
    bst_time = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
    
    start = clock();
    avl_node_t* avl_result = avl_search(avl_root, word);
    end = clock();
    avl_time = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
    
    start = clock();
    int chain_result = hash_chain_search(chain_ht, word, &chain_comparisons);
    end = clock();
    chain_time = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
    
    start = clock();
    int open_result = hash_open_search(open_ht, word, &open_comparisons);
    end = clock();
    open_time = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
    
    printf("BST: %s %.2f мкс\n", bst_result ? "найдено" : "нет", bst_time);
    printf("AVL: %s %.2f мкс\n", avl_result ? "найдено" : "нет", avl_time);
    printf("Хеш(цеп): %s %.2f мкс сравн: %d\n", chain_result ? "найдено" : "нет", chain_time, chain_comparisons);
    printf("Хеш(откр): %s %.2f мкс сравн: %d\n", open_result ? "найдено" : "нет", open_time, open_comparisons);
}

void delete_word(void) {
    if (!bst_root) {
        printf("Данные не загружены\n");
        return;
    }
    
    char word[100];
    printf("Удалить слово: ");
    fgets(word, sizeof(word), stdin);
    word[strcspn(word, "\n")] = 0;
    
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
    
    if (!tree_search(bst_root, word)) {
        printf("Слово не найдено\n");
        return;
    }
    
    bst_root = tree_delete(bst_root, word);
    avl_root = avl_delete(avl_root, word);
    hash_chain_delete(chain_ht, word);
    hash_open_delete(open_ht, word);
    
    printf("Удалено\n");
}
void compare_performance(void) {
    if (!bst_root) {
        printf("Данные не загружены\n");
        return;
    }
    
    printf("Сравнение (100 операций):\n");
    
    
    if (chain_ht && hash_needs_rehash(chain_ht, 5)) {
        hash_chain_rehash(&chain_ht);
    }
    
    if (open_ht) {
        if ((float)open_ht->count / open_ht->size > 0.5) {
            hash_open_rehash(&open_ht);
        }
    }
    
    
    int word_count = tree_count_nodes(bst_root);
    if (word_count == 0) {
        printf("Нет слов для тестирования\n");
        return;
    }
    
    char** words_array = NULL;
    int index = 0;
    int capacity = word_count;
    
    words_array = malloc(capacity * sizeof(char*));
    if (!words_array) {
        printf("Ошибка выделения памяти\n");
        return;
    }
    
    
    collect_words_for_test(bst_root, &words_array, &index, &capacity);
    
    
    srand(time(NULL));
    
    double bst_total = 0, avl_total = 0, chain_total = 0, open_total = 0;
    int total_chain_comparisons = 0, total_open_comparisons = 0;
    
    
    int tests = (100 < index) ? 100 : index;
    
    for (int i = 0; i < tests; i++) {
        int random_index = rand() % index;
        char* word = words_array[random_index];
        
        clock_t start = clock();
        tree_search(bst_root, word);
        clock_t end = clock();
        bst_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        
        start = clock();
        avl_search(avl_root, word);
        end = clock();
        avl_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        
        int chain_comparisons;
        start = clock();
        hash_chain_search(chain_ht, word, &chain_comparisons);
        end = clock();
        chain_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        total_chain_comparisons += chain_comparisons;
        
        int open_comparisons;
        start = clock();
        hash_open_search(open_ht, word, &open_comparisons);
        end = clock();
        open_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        total_open_comparisons += open_comparisons;
    }
    
    
    free(words_array);
    
    
    printf("\nТекущие параметры таблиц:\n");
    printf("Цепочки: размер=%d, элементов=%d, загрузка=%.1f%%\n", 
           chain_ht->size, chain_ht->count, (float)chain_ht->count/chain_ht->size*100);
    printf("Открытая: размер=%d, элементов=%d, загрузка=%.1f%%\n", 
           open_ht->size, open_ht->count, (float)open_ht->count/open_ht->size*100);
    
    printf("\nРезультаты (%d операций поиска):\n", tests);
    printf("BST: %.2f мкс в среднем\n", bst_total / tests);
    printf("AVL: %.2f мкс в среднем\n", avl_total / tests);
    printf("Хеш(цеп): %.2f мкс ср длина коллизии: %.1f\n", chain_total / tests, total_chain_comparisons / (double)tests);
    printf("Хеш(откр): %.2f мкс ср длина коллизии: %.1f\n", open_total / tests, total_open_comparisons / (double)tests);
}
void compare_performance1(void) {
    if (!bst_root) {
        printf("Данные не загружены\n");
        return;
    }
    
    printf("Сравнение времени УДАЛЕНИЯ (10 операций):\n");
    
    
    if (chain_ht && hash_needs_rehash(chain_ht, 5)) {
        printf("Перехеширование цепочечной таблицы...\n");
        hash_chain_rehash(&chain_ht);
    }
    
    if (open_ht && hash_open_needs_rehash(open_ht, 5)) {
        printf("Перехеширование таблицы с открытой адресацией...\n");
        hash_open_rehash(&open_ht);
    }
    
    
    int word_count = tree_count_nodes(bst_root);
    if (word_count == 0) {
        printf("Нет слов для тестирования\n");
        return;
    }
    
    char** words_array = malloc(word_count * sizeof(char*));
    if (!words_array) {
        printf("Ошибка выделения памяти\n");
        return;
    }
    
    int index = 0;
    int capacity = word_count;
    
    
    collect_words_for_test(bst_root, &words_array, &index, &capacity);
    
    
    srand(time(NULL));
    
    double bst_total = 0, avl_total = 0, chain_total = 0, open_total = 0;
    int bst_deleted_total = 0, avl_deleted_total = 0;
    int chain_deleted_total = 0, open_deleted_total = 0;
    
    
    int tests = 10;
    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    
    for (int i = 0; i < tests; i++) {
        
        char letter = letters[rand() % 26];
        printf("Тест %d: удаление слов на букву '%c'\n", i + 1, letter);
        
        int bst_deleted = 0, avl_deleted = 0;
        int chain_deleted = 0, open_deleted = 0;
        
        
        clock_t start = clock();
        tree_delete_by_letter(bst_root, letter, &bst_deleted);
        clock_t end = clock();
        bst_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        bst_deleted_total += bst_deleted;
        
        
        start = clock();
        avl_delete_by_letter(avl_root, letter, &avl_deleted);
        end = clock();
        avl_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        avl_deleted_total += avl_deleted;
        
        
        start = clock();
        hash_chain_delete_by_letter(chain_ht, letter, &chain_deleted);
        end = clock();
        chain_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        chain_deleted_total += chain_deleted;
        
        
        start = clock();
        hash_open_delete_by_letter(open_ht, letter, &open_deleted);
        end = clock();
        open_total += (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
        open_deleted_total += open_deleted;
        
        
        
        
        printf("  Удалено: BST=%d, AVL=%d, Цепочки=%d, Открытая=%d\n", 
               bst_deleted, avl_deleted, chain_deleted, open_deleted);
    }
    
    
    free(words_array);
    
    
    printf("\n РЕЗУЛЬТАТЫ СРАВНЕНИЯ УДАЛЕНИЯ \n");
    printf("Всего выполнено: %d операций удаления по разным буквам\n", tests);
    printf("Всего удалено слов:\n");
    printf("  BST: %d (в среднем %.1f за операцию)\n", 
           bst_deleted_total, (float)bst_deleted_total / tests);
    printf("  AVL: %d (в среднем %.1f за операцию)\n", 
           avl_deleted_total, (float)avl_deleted_total / tests);
    printf("  Хеш(цеп): %d (в среднем %.1f за операцию)\n", 
           chain_deleted_total, (float)chain_deleted_total / tests);
    printf("  Хеш(откр): %d (в среднем %.1f за операцию)\n", 
           open_deleted_total, (float)open_deleted_total / tests);
    
    printf("\nСреднее время удаления (на одну операцию):\n");
    printf("  BST: %.2f мкс\n", bst_total / tests);
    printf("  AVL: %.2f мкс\n", avl_total / tests);
    printf("  Хеш(цеп): %.2f мкс\n", chain_total / tests);
    printf("  Хеш(откр): %.2f мкс\n", open_total / tests);
    
    printf("\nОтносительная производительность:\n");
    double base_time = bst_total / tests;
    printf("  AVL быстрее BST в %.2f раз\n", base_time / (avl_total / tests));
    printf("  Хеш(цеп) быстрее BST в %.2f раз\n", base_time / (chain_total / tests));
    printf("  Хеш(откр) быстрее BST в %.2f раз\n", base_time / (open_total / tests));
}
void delete_by_letter(void) {
    if (!bst_root) {
        printf("Данные не загружены\n");
        return;
    }
    
    char letter;
    printf("Удалить слова на букву: ");
    scanf("%c", &letter);
    clean_input_buffer();
    letter = tolower(letter);
    
    int bst_deleted = 0, avl_deleted = 0, chain_deleted = 0, open_deleted = 0;
    
    clock_t start = clock();
    bst_root = tree_delete_by_letter(bst_root, letter, &bst_deleted);
    clock_t end = clock();
    double bst_time = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
    
    start = clock();
    avl_root = avl_delete_by_letter(avl_root, letter, &avl_deleted);
    end = clock();
    double avl_time = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
    
    start = clock();
    hash_chain_delete_by_letter(chain_ht, letter, &chain_deleted);
    end = clock();
    double chain_time = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
    
    start = clock();
    hash_open_delete_by_letter(open_ht, letter, &open_deleted);
    end = clock();
    double open_time = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;
    
    printf("BST: %d слов %.2f мкс\n", bst_deleted, bst_time);
    printf("AVL: %d слов %.2f мкс\n", avl_deleted, avl_time);
    printf("Хеш(цеп): %d слов %.2f мкс\n", chain_deleted, chain_time);
    printf("Хеш(откр): %d слов %.2f мкс\n", open_deleted, open_time);
}


void show_structures(void) {
    if (!bst_root) {
        printf("Данные не загружены\n");
        return;
    }
    
    printf("BST (высота: %d):\n", tree_height(bst_root));
    tree_print_visual(bst_root, 0);
    
    printf("\nAVL (высота: %d):\n", avl_height(avl_root));
    avl_print_visual(avl_root, 0);
    
    printf("\nХеш-таблица (цепочки):\n");
    for (int i = 0; i < chain_ht->size; i++) {
        printf("[%d]: ", i);
        hash_node_chain_t* current = chain_ht->table[i];
        if (!current) {
            printf("пусто\n");
            continue;
        }
        while (current) {
            printf("%s", current->word);
            if (current->next) printf(" -> ");
            current = current->next;
        }
        printf("\n");
    }
    
    printf("\nХеш-таблица (открытая адресация):\n");
    for (int i = 0; i < open_ht->size; i++) {
        printf("[%d]: ", i);
        if (open_ht->table[i].is_occupied) {
            printf("%s %d\n", open_ht->table[i].word, hash_function(open_ht->table[i].word,open_ht->size));
        } else {
            printf("свободно\n");
        }
    }
}

void export_dot(void) {
    if (!bst_root) {
        printf("Данные не загружены\n");
        return;
    }
    
    bst_to_dot(bst_root, "bst.dot");
    avl_to_dot(avl_root, "avl.dot");
    
    FILE* f = fopen("compare.dot", "w");
    if (f) {
        fprintf(f, "digraph Comparison {\n");
        fprintf(f, "    rankdir=TB;\n");
        fprintf(f, "    node [shape=circle];\n\n");
        
        fprintf(f, "    subgraph cluster_BST {\n");
        fprintf(f, "        label=\"BST\";\n");
        print_bst_for_dot(bst_root, f);
        fprintf(f, "    }\n\n");
        
        fprintf(f, "    subgraph cluster_AVL {\n");
        fprintf(f, "        label=\"AVL\";\n");
        print_avl_for_dot(avl_root, f);
        fprintf(f, "    }\n");
        fprintf(f, "}\n");
        fclose(f);
    }
    
    printf("Файлы созданы:\n");
    printf("bst.dot\n");
    printf("avl.dot\n");
    printf("compare.dot\n");
    printf("Для создания PNG: dot -Tpng файл.dot -o файл.png\n");
}

int main() {
    int choice;
    
    do {
        printf("\n1. Загрузить данные\n");
        printf("2. Поиск слова\n");
        printf("3. Удалить слово\n");
        printf("4. Удалить слова на букву\n");
        printf("5. Сравнить производительность\n");
        printf("6. Показать структуры\n");
        printf("7. Экспорт в DOT\n");
        printf("8. Сбалансировать BST\n");
        printf("9. Показать статистику памяти\n");
        printf("10. Проверить и перехешировать таблицы\n");
        printf("11. Тестирование разных размеров таблиц\n");
        printf("12. Сравнить эффективность при разных коллизиях\n");
        printf("0. Выход\n");
        printf("> ");
        
        if (scanf("%d", &choice) != 1) {
            clean_input_buffer();
            continue;
        }
        clean_input_buffer();
        
        switch (choice) {
            case 1: load_data(); break;
            case 2: search_word(); break;
            case 3: delete_word(); break;
            case 4: delete_by_letter(); break;
            case 5: 
                compare_performance();
                compare_performance1();
                break;
            case 6: show_structures(); break;
            case 7: export_dot(); break;
            case 8:
                if (bst_root) {
                    printf("Балансировка BST...\n");
                    bst_root = tree_balance(bst_root);
                    printf("BST сбалансирован. Новая высота: %d\n", tree_height(bst_root));
                } else {
                    printf("Данные не загружены\n");
                }
                break;
                
            case 9:
                if (bst_root) {
                    print_detailed_memory_comparison(bst_root, avl_root, chain_ht, open_ht);
                } else {
                    printf("Данные не загружены\n");
                }
                break;
                
            case 10:
                if (chain_ht && open_ht) {
                    int max_comparisons;
                    printf("Максимальное допустимое количество сравнений: ");
                    scanf("%d", &max_comparisons);
                    clean_input_buffer();
                    
                    if (hash_needs_rehash(chain_ht, max_comparisons)) {
                        printf("Цепочечная таблица требует перехеширования...\n");
                        hash_chain_rehash(&chain_ht);
                    } else {
                        printf("Цепочечная таблица не требует перехеширования\n");
                    }
                    
                    if (hash_open_needs_rehash(open_ht, max_comparisons)) {
                        printf("Таблица с открытой адресацией требует перехеширования...\n");
                        hash_open_rehash(&open_ht);
                    } else {
                        printf("Таблица с открытой адресацией не требует перехеширования\n");
                    }
                } else {
                    printf("Данные не загружены\n");
                }
                break;
                
            case 11:
                test_different_table_sizes();
                break;
                
            case 12:
                
                analyze_collision_efficiency();
                break;
            case 0: break;
            default: printf("Неверный выбор\n");
        }
        
    } while (choice != 0);
    
    tree_free(bst_root);
    avl_free(avl_root);
    if (chain_ht) hash_chain_free(chain_ht);
    if (open_ht) hash_open_free(open_ht);
    
    return 0;
}