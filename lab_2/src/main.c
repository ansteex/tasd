#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "io_struct.h"
#include "read_from_file.h"
#include "sort.h"
#include "ops.h"
#include "keys_ops.h"
#define MAX_LEN 100
#define AGE 4
#define MAX_TABLE_SIZE 500
#define ERROR 1
void print_menu() {
    printf("\n=== МЕНЮ ===\n");
    printf("1. Добавить спектакль\n");
    printf("2. Удалить спектакль\n");
    printf("3. Просмотреть таблицу\n");
    printf("4. Создать таблицу ключей\n");
    printf("5. Просмотреть таблицу ключей\n");
    printf("6. Сортировка таблицы пузырьком\n");
    printf("7. Сортировка таблицы быстрой сортировкой\n");
    printf("8. Сортировка ключей пузырьком\n");
    printf("9. Сортировка ключей быстрой сортировкой\n");
    printf("10. Просмотр по ключам\n");
    printf("11. Поиск балетов\n");
    printf("12. Генерация тестовых данных\n");
    printf("13. Сравнение эффективности сортировок\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

int main() {
    struct Spectacle spectacle_table[MAX_TABLE_SIZE];
    struct KeyRecord key_table[MAX_TABLE_SIZE];
    int spectacle_count = 0;
    int key_count = 0;
    int choice;
    char search_name[MAX_LEN];
    do {
        print_menu();
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch(choice) {
            case 1:
                add_spectacle(spectacle_table, &spectacle_count);
                break;
            case 2:
                printf("Введите название спектакля для удаления: ");
                fgets(search_name, sizeof(search_name), stdin);
                search_name[strcspn(search_name, "\n")] = 0;
                int index = find_spectacle_by_name(search_name, spectacle_table, spectacle_count);
                if (index != -1) {
                    delete_spectacle(index, spectacle_table, &spectacle_count);
                } else {
                    printf("Спектакль не найден\n");
                }
                break;
            case 3:
                print_table(spectacle_table, spectacle_count);
                break;
            case 4:
                create_key_table(spectacle_table, spectacle_count, key_table, &key_count);
                break;
            case 5:
                print_key_table(key_count,key_table);
                break;
            case 6:
                bubble_sort_spectacles( spectacle_table, spectacle_count);
                break;
            case 7:
                quick_sort_spectacles(0, spectacle_count - 1, spectacle_table, spectacle_count);
                break;
            case 8:
                bubble_sort_keys(key_table, key_count);
                break;
            case 9:
                quick_sort_keys(0, key_count - 1, key_table, key_count);
                break;
            case 10:
                print_table_by_keys(spectacle_table, key_table, key_count);
                break;
            case 11:
                find_ballet(spectacle_table, spectacle_count);
                break;
            case 12:
                generate_test_data(spectacle_table, &spectacle_count);
                break;
            case 13:
                compare_efficiency(spectacle_table, spectacle_count, key_table, key_count);
                break;
            case 0:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Нет такого варианта\n");
        }
    } while (choice != 0);
    
    return 0;
}
