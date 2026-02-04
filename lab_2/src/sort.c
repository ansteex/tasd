#include "sort.h"

void bubble_sort_spectacles(struct Spectacle *spectacle_table, int spectacle_count) {
    if (spectacle_count == 0) {
        printf("Таблица пуста.\n");
        return;
    }
    for (int i = 0; i < spectacle_count - 1; i++) {
        for (int j = 0; j < spectacle_count - i - 1; j++) {
            if (strcmp(spectacle_table[j].spec_name, spectacle_table[j + 1].spec_name) > 0) {
                struct Spectacle temp = spectacle_table[j];
                spectacle_table[j] = spectacle_table[j + 1];
                spectacle_table[j + 1] = temp;
            }
        }
    }
}

void bubble_sort_keys(struct KeyRecord *key_table, int key_count) {
    if (key_count == 0) {
        printf("Таблица пуста.\n");
        return;
    }
    for (int i = 0; i < key_count - 1; i++) {
        for (int j = 0; j < key_count - i - 1; j++) {
            if (strcmp(key_table[j].key_value, key_table[j + 1].key_value) > 0) {
                struct KeyRecord temp = key_table[j];
                key_table[j] = key_table[j + 1];
                key_table[j + 1] = temp;
            }
        }
    }
}

void quick_sort_spectacles(int left, int right, struct Spectacle *spectacle_table, int spectacle_count) {
    if (spectacle_count == 0) {
        printf("Таблица ключей пуста.\n");
        return;
    }
    if (left >= right) return;
    
    int i = left, j = right;
    char pivot[MAX_LEN];
    strcpy(pivot, spectacle_table[(left + right) / 2].spec_name);
    
    while (i <= j) {
        while (strcmp(spectacle_table[i].spec_name, pivot) < 0) i++;
        while (strcmp(spectacle_table[j].spec_name, pivot) > 0) j--;
        if (i <= j) {
            struct Spectacle temp = spectacle_table[i];
            spectacle_table[i] = spectacle_table[j];
            spectacle_table[j] = temp;
            i++;
            j--;
        }
    }
    
    quick_sort_spectacles(left, j, spectacle_table, spectacle_count);
    quick_sort_spectacles(i, right, spectacle_table, spectacle_count);
}

void quick_sort_keys(int left, int right, struct KeyRecord *key_table, int key_count) {
    if (key_count == 0) {
        printf("Таблица ключей пуста.\n");
        return;
    }
    if (left >= right) return;
    
    int i = left, j = right;
    char pivot[MAX_LEN];
    strcpy(pivot, key_table[(left + right) / 2].key_value);
    
    while (i <= j) {
        while (strcmp(key_table[i].key_value, pivot) < 0) i++;
        while (strcmp(key_table[j].key_value, pivot) > 0) j--;
        if (i <= j) {
            struct KeyRecord temp = key_table[i];
            key_table[i] = key_table[j];
            key_table[j] = temp;
            i++;
            j--;
        }
    }
    
    quick_sort_keys(left, j, key_table, key_count);
    quick_sort_keys(i, right, key_table, key_count);
}
int get_array_type(void *array, int count) {
    if (count == 0 || array == NULL) return 0;
    for (int i = 0; i < (count < 3 ? count : 3); i++) {
        struct Spectacle *spec = (struct Spectacle*)array;
        struct KeyRecord *key = (struct KeyRecord*)array;
        if (i == 0) {
            if (strlen(spec[i].spec_name) > 0 && spec[i].spec_name[0] != '\0') {
                return 1;
            }
            if (strlen(key[i].key_value) > 0 && key[i].key_value[0] != '\0') {
                return 2;
            }
        }
    }
    return 0;
}

double measure_sorting_time(void (*sort_func)(void*, int), void *array, int count, int is_quick_sort, int array_type) {
    const int NUM_MEASUREMENTS = 50;
    double total_time = 0.0;
    
    if (count == 0 || array == NULL) {
        return 0.0;
    }
    
    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        void *temp_array;
        size_t element_size;
        
        if (array_type == 1) {
            element_size = sizeof(struct Spectacle);
        } else {
            element_size = sizeof(struct KeyRecord);
        }
        
        temp_array = malloc(count * element_size);
        if (temp_array == NULL) {
            printf("Ошибка выделения памяти\n");
            return 0.0;
        }
        
        memcpy(temp_array, array, count * element_size);
        
        clock_t start = clock();
        
        if (is_quick_sort) {
            if (array_type == 1) {
                quick_sort_spectacles(0, count - 1, (struct Spectacle*)temp_array, count);
            } else {
                quick_sort_keys(0, count - 1, (struct KeyRecord*)temp_array, count);
            }
        } else {
            sort_func(temp_array, count);
        }
        
        clock_t end = clock();
        total_time += ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        
        free(temp_array);
    }
    
    return total_time / NUM_MEASUREMENTS;
}

double bubble_sort_spectacles_with_time(struct Spectacle *spectacle_table, int spectacle_count) {
    return measure_sorting_time((void (*)(void*, int))bubble_sort_spectacles, 
                               spectacle_table, spectacle_count, 0, 1);
}

double bubble_sort_keys_with_time(struct KeyRecord *key_table, int key_count) {
    return measure_sorting_time((void (*)(void*, int))bubble_sort_keys, 
                               key_table, key_count, 0, 2);
}

double quick_sort_spectacles_with_time(struct Spectacle *spectacle_table, int spectacle_count) {
    return measure_sorting_time(NULL, spectacle_table, spectacle_count, 1, 1);
}

double quick_sort_keys_with_time(struct KeyRecord *key_table, int key_count) {
    return measure_sorting_time(NULL, key_table, key_count, 1, 2);
}

void compare_efficiency(struct Spectacle *spectacle_table, int spectacle_count, 
                       struct KeyRecord *key_table, int key_count) {
    printf("\nСРАВНЕНИЕ ЭФФЕКТИВНОСТИ СОРТИРОВОК\n");    
    if (spectacle_count == 0) {
        printf("Таблица пуста.\n");
        return;
    }
    struct Spectacle *spectacle_copy = malloc(spectacle_count * sizeof(struct Spectacle));
    struct KeyRecord *key_copy = malloc(key_count * sizeof(struct KeyRecord));
    
    if (spectacle_copy == NULL || key_copy == NULL) {
        printf("Ошибка выделения памяти\n");
        free(spectacle_copy);
        free(key_copy);
        return;
    }
    
    memcpy(spectacle_copy, spectacle_table, spectacle_count * sizeof(struct Spectacle));
    memcpy(key_copy, key_table, key_count * sizeof(struct KeyRecord));
    double time_bubble_spectacles = bubble_sort_spectacles_with_time(spectacle_copy, spectacle_count);
    double time_quick_spectacles = quick_sort_spectacles_with_time(spectacle_table, spectacle_count);
    double time_bubble_keys = bubble_sort_keys_with_time(key_copy, key_count);
    double time_quick_keys = quick_sort_keys_with_time(key_table, key_count);
    memcpy(spectacle_table, spectacle_copy, spectacle_count * sizeof(struct Spectacle));
    memcpy(key_table, key_copy, key_count * sizeof(struct KeyRecord));
    
    printf("РЕЗУЛЬТАТЫ ЗАМЕРОВ ВРЕМЕНИ\n");
    printf("Сортировка основной таблицы:\n");
    printf("  Пузырьковая: %.3f мс\n", time_bubble_spectacles);
    printf("  Быстрая:     %.3f мс\n", time_quick_spectacles);
    
    printf("\nСортировка таблицы ключей:\n");
    printf("  Пузырьковая: %.3f мс\n", time_bubble_keys);
    printf("  Быстрая:     %.3f мс\n", time_quick_keys);
    
    size_t memory_spectacles = spectacle_count * sizeof(struct Spectacle);
    size_t memory_keys = key_count * sizeof(struct KeyRecord);
    
    printf("\nИСПОЛЬЗОВАНИЕ ПАМЯТИ\n");
    printf("Основная таблица: %zu байт\n", memory_spectacles);
    printf("Таблица ключей:   %zu байт\n", memory_keys);
    printf("Экономия памяти при использовании ключей: %.1f%%\n", 
           ((double)(memory_spectacles - memory_keys) / memory_spectacles) * 100);
    
    free(spectacle_copy);
    free(key_copy);
}