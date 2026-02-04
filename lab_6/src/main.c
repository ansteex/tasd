#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void clean_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu(void) {
    printf("Меню\n");
    printf("1 - Загрузить слова из файла в дерево\n");
    printf("2 - Добавить слово в дерево\n");
    printf("3 - Удалить слово из дерева\n");
    printf("4 - Найти слово в дереве\n");
    printf("5 - Удалить слова, начинающиеся на букву\n");
    printf("6 - Префиксный обход (корень -> левое -> правое)\n");
    printf("7 - Инфиксный обход (левое -> корень -> правое)\n");
    printf("8 - Постфиксный обход (левое -> правое -> корень)\n");
    printf("9 - Показать дерево\n");
    printf("10 - Экспорт в DOT формат (для Graphviz)\n");
    printf("11 - Сравнить время удаления\n");
    printf("12 - Показать высоту дерева\n");
    printf("0 - Выход\n");
    printf("Выберите действие: ");
}

int main() {
    tree_node_t* root = NULL;
    char filename[MAX_FILENAME];
    char current_filename[MAX_FILENAME] = "";
    char word[MAX_WORD_LENGTH];
    char letter;
    int choice;

    MAIN_MENU:
    print_menu();
    
    if (scanf("%d", &choice) != 1) {
        printf("Ошибка ввода\n");
        clean_input_buffer();
        goto MAIN_MENU;
    }
    clean_input_buffer();

    switch (choice) {
        case 1: 
            printf("Введите имя файла: ");
            if (fgets(filename, sizeof(filename), stdin) == NULL) {
                printf("Ошибка чтения ввода\n");
                break;
            }
            filename[strcspn(filename, "\n")] = 0;

            tree_free(root);
            root = NULL;

            if (tree_read_from_file(filename, &root) != 0) {
                printf("Ошибка загрузки файла\n");
                break;
            }

            strcpy(current_filename, filename);
            printf("Дерево успешно построено из файла %s\n", filename);
            break;

        case 2: 
            printf("Введите слово для добавления: ");
            if (fgets(word, sizeof(word), stdin) == NULL) {
                printf("Ошибка чтения ввода\n");
                break;
            }
            word[strcspn(word, "\n")] = 0;
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }
            
            if (strlen(word) == 0) {
                printf("Ошибка: пустое слово\n");
                break;
            }
            
            root = tree_insert(root, word);
            printf("Слово '%s' добавлено в дерево\n", word);
            break;

        case 3: 
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }
            
            printf("Введите слово для удаления: ");
            if (fgets(word, sizeof(word), stdin) == NULL) {
                printf("Ошибка чтения ввода\n");
                break;
            }
            word[strcspn(word, "\n")] = 0;
            
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }
            
            tree_node_t* found = tree_search(root, word);
            if (found == NULL) {
                printf("Слово '%s' не найдено в дереве\n", word);
                break;
            }
            
            root = tree_delete(root, word);
            printf("Слово '%s' удалено из дерева\n", word);
            break;

        case 4:
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }
            
            printf("Введите слово для поиска: ");
            if (fgets(word, sizeof(word), stdin) == NULL) {
                printf("Ошибка чтения ввода\n");
                break;
            }
            word[strcspn(word, "\n")] = 0;
            
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }
            
            found = tree_search(root, word);
            if (found) {
                printf("Слово '%s' найдено в дереве\n", word);
            } else {
                printf("Слово '%s' не найдено в дереве\n", word);
            }
            break;

        case 5:
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }

            printf("Введите букву: ");
            if (scanf("%c", &letter) != 1) {
                printf("Ошибка ввода\n");
                clean_input_buffer();
                break;
            }
            clean_input_buffer();

            int deleted_count = 0;
            root = tree_delete_by_letter(root, tolower(letter),&deleted_count);

            if(deleted_count==0)
                printf("Не нашлось таких слов\n");
            else 
                printf("Удалено %d слов\n", deleted_count);
            break;

        case 6:
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }
            printf("Префиксный обход: ");
            tree_preorder_traversal(root);
            printf("\n");
            break;

        case 7: 
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }
            printf("Инфиксный обход: ");
            tree_inorder_traversal(root);
            printf("\n");
            break;

        case 8: 
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }
            printf("Постфиксный обход: ");
            tree_postorder_traversal(root);
            printf("\n");
            break;

        case 9:
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }
            printf("Текстовое представление дерева:\n");
            tree_print_visual(root, 0);
            break;

        case 10: 
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }
            printf("Введите имя для DOT файла (например: tree.dot): ");
            if (fgets(filename, sizeof(filename), stdin) == NULL) {
                printf("Ошибка чтения ввода\n");
                break;
            }
            filename[strcspn(filename, "\n")] = 0;
            
            tree_export_to_dot(root, filename);
            break;

        case 11:
            if (root == NULL || strlen(current_filename) == 0) {
                printf("Сначала загрузите файл\n");
                break;
            }

            printf("Введите букву: ");
            if (scanf("%c", &letter) != 1) {
                printf("Ошибка ввода\n");
                clean_input_buffer();
                break;
            }
            clean_input_buffer();
            tree_node_t* original_tree = tree_copy(root);
            if (original_tree == NULL) {
                printf("Ошибка: не удалось создать копию дерева\n");
                break;
            }
            char temp_source_filename[]="1.txt";
            FILE* src = fopen(current_filename, "r");
            if (src) {
                FILE* dst = fopen(temp_source_filename, "w");
                if (dst) {
                    char buffer[MAX_LINE_LENGTH];
                    while (fgets(buffer, sizeof(buffer), src)) {
                        fputs(buffer, dst);
                    }
                    fclose(dst);
                }
                fclose(src);
            }

            double tree_time = 0;
            for (size_t i = 0; i < 100; i++) {
                tree_node_t* test_tree = tree_copy(original_tree);
                if (test_tree == NULL) {
                    printf("Ошибка копирования дерева на итерации %zu\n", i);
                    break;
                }
                
                tree_time += tree_measure_deletion_time(&test_tree, tolower(letter));
                tree_free(test_tree);
            }
            tree_time /= 100;

            double file_time = 0;
            for (size_t i = 0; i < 100; i++) {
                src = fopen(temp_source_filename, "r");
                if (src) {
                    FILE* dst = fopen(current_filename, "w");
                    if (dst) {
                        char buffer[MAX_LINE_LENGTH];
                        while (fgets(buffer, sizeof(buffer), src)) {
                            fputs(buffer, dst);
                        }
                        fclose(dst);
                    }
                    fclose(src);
                }
                
                file_time += file_measure_deletion_time(current_filename, tolower(letter));
            }
            file_time /= 100;
            tree_free(root);
            root = tree_copy(original_tree);
            tree_free(original_tree);
            remove(temp_source_filename);

            printf("\n=== Сравнение времени удаления (100 итераций) ===\n");
            printf("Дерево: %.3f мкс\n", tree_time);
            printf("Файл:   %.3f мкс\n", file_time);
            
            if (file_time > 0) {
                double efficiency = (file_time - tree_time) / file_time * 100;
                printf("Эффективность дерева: %.1f%%\n", efficiency);
            }
            break;
        case 12: 
            if (root == NULL) {
                printf("Дерево пусто\n");
                break;
            }
            printf("Высота дерева: %d\n", tree_height(root));
            break;
        case 0:
            printf("Завершение работы\n");
            break;

        default:
            printf("Неверный выбор\n");
            break;
    }

    if (choice != 0) {
        goto MAIN_MENU;
    }

    tree_free(root);
    return 0;
}