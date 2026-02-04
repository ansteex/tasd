#include "ops.h"

void print_table(struct Spectacle *spectacle_table,int spectacle_count) {
    if (spectacle_count == 0) {
        printf("Пустая таблица\n");
        return;
    }
    for (int i = 0; i < spectacle_count; i++) {
        print_spectacle(&spectacle_table[i], i);
    }
}
void add_spectacle(struct Spectacle *spectacle_table,int *spectacle_count) {
    if (*spectacle_count >= MAX_TABLE_SIZE) {
        printf("Таблица переполнена\n");
        return;
    }
    input_spectacle(&spectacle_table[*spectacle_count]);
    (*spectacle_count)++;
}

void delete_spectacle(int index,struct Spectacle *spectacle_table,int *spectacle_count) {
    if (index < 0 || index >= *spectacle_count) {
        printf("Неверный индекс\n");
        return;
    }
    for (int i = index; i < *spectacle_count - 1; i++) {
        spectacle_table[i] = spectacle_table[i + 1];
    }
    (*spectacle_count)--;
    printf("Спектакль удален\n");
}

int find_spectacle_by_name(char *name,struct Spectacle *spectacle_table,int spectacle_count) {
    for (int i = 0; i < spectacle_count; i++) {
        if (strcmp(spectacle_table[i].spec_name, name) == 0) {
            return i;
        }
    }
    return NOT_FOUND;
}
void find_ballet(struct Spectacle *spectacle_table,int spectacle_count) {
    enum AGE_LIMIT target_age;
    int max_duration;
    
    int valid_input = 0;
        while (!valid_input) {
            printf("Возрастная категория (3+, 10+, 16+): ");
            char s_age[AGE];
            if (fgets(s_age, sizeof(s_age), stdin)) {
                target_age= age_limit_from_string(s_age);
                if (target_age != AGE_ERROR) {
                    valid_input = 1;
                }
            }
        }
    valid_input = 0;
        while (!valid_input) {
            printf("Введите максимальную продолжительность (минут): ");
            if (scanf("%d", &max_duration) == 1 && max_duration > 0) {
                valid_input = 1;
            } else {
                clear_input_buffer();
            }
        }
    clear_input_buffer();
    
    printf("\nРЕЗУЛЬТАТЫ ПОИСКА БАЛЕТОВ\n");
    int found = 0;
    for (int i = 0; i < spectacle_count; i++) {
        if (spectacle_table[i].type == 1 &&
            spectacle_table[i].musical.musical_type == 0 &&
            spectacle_table[i].musical.age == target_age && 
            spectacle_table[i].musical.duration < max_duration) {
            
            print_spectacle(&spectacle_table[i], i);
            found++;
        }
    }
    
    if (!found) {
        printf("Балеты по заданным критериям не найдены.\n");
    }
}

void generate_test_data(struct Spectacle *spectacle_table, int *spectacle_count) {
    FILE *f = fopen("data.txt", "r");
    if (f == NULL) {
        printf("Ошибка: не удалось открыть файл data.txt\n");
        return;
    }
    
    int records_added = 0;
    for (int i = 0; i < MAX_TABLE_SIZE && *spectacle_count < MAX_TABLE_SIZE; i++) {
        if (input_spectacle_from_file(f, &spectacle_table[*spectacle_count]) == 0) {
            (*spectacle_count)++;
            records_added++;
        } else {
            printf("Ошибка чтения записи %d из файла\n", i + 1);
            break;
        }
    }
    
    fclose(f);
    printf("Добавлено %d записей из файла\n", records_added);
}
