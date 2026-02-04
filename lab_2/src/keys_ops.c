#include "keys_ops.h"
void create_key_table(struct Spectacle *spectacle_table, int spectacle_count, struct KeyRecord *key_table, int *key_count) {
    if(spectacle_count==0)
    {
        printf("Таблица пуста, невозможно создать таблицу ключей\n");
        return;
    }
    *key_count = spectacle_count;
    for (int i = 0; i < spectacle_count; i++) {
        strcpy(key_table[i].key_value, spectacle_table[i].spec_name);
        key_table[i].original_index = i;
    }
}
void print_key_table(int key_count ,struct KeyRecord *key_table) {
    if (key_count == 0) {
        printf("Таблица ключей пуста\n");
        return;
    }
    for (int i = 0; i < key_count; i++) {
        printf("%d. Ключ: %-30s Индекс: %d\n", 
               i + 1, key_table[i].key_value, key_table[i].original_index + 1);
    }
}
void print_table_by_keys(struct Spectacle *spectacle_table, struct KeyRecord *key_table, int key_count) {
    if (key_count == 0) {
        printf("Сначала создайте таблицу ключей\n");
        return;
    }
    for (int i = 0; i < key_count; i++) {
        int idx = key_table[i].original_index;
        print_spectacle(&spectacle_table[idx], i);
    }
}