#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
int hash_function(const char* word, int table_size) {
    unsigned int hash = 0;
    
    for (int i = 0; word[i] != '\0'; i++) {
        hash = (hash << 4) ^ (hash >> 28) ^ word[i];
    }
    
    return hash % table_size;
}
hash_table_chain_t* hash_chain_create(int size) {
    hash_table_chain_t* ht = malloc(sizeof(hash_table_chain_t));
    if (!ht) return NULL;
    
    ht->size = size;
    ht->count = 0;
    ht->collisions = 0;
    ht->table = calloc(size, sizeof(hash_node_chain_t*));
    
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    
    return ht;
}

void hash_chain_insert(hash_table_chain_t* ht, const char* word) {
    if (!ht || ht->count >= ht->size) return;
    
    int index = hash_function(word, ht->size);
    hash_node_chain_t* new_node = malloc(sizeof(hash_node_chain_t));
    if (!new_node) return;
    
    strcpy(new_node->word, word);
    new_node->next = NULL;
    
    if (!ht->table[index]) {
        ht->table[index] = new_node;
    } else {
        ht->collisions++;
        hash_node_chain_t* current = ht->table[index];
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    ht->count++;
}

int hash_chain_search(hash_table_chain_t* ht, const char* word, int* comparisons) {
    if (!ht || !word) return 0;
    
    int index = hash_function(word, ht->size);
    hash_node_chain_t* current = ht->table[index];
    *comparisons = 0;
    
    while (current) {
        (*comparisons)++;
        if (strcmp(current->word, word) == 0) {
            return 1;
        }
        current = current->next;
    }
    
    return 0;
}

void hash_chain_delete(hash_table_chain_t* ht, const char* word) {
    if (!ht) return;
    
    int index = hash_function(word, ht->size);
    hash_node_chain_t* current = ht->table[index];
    hash_node_chain_t* prev = NULL;
    
    while (current) {
        if (strcmp(current->word, word) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                ht->table[index] = current->next;
            }
            
            free(current);
            ht->count--;
            return;
        }
        
        prev = current;
        current = current->next;
    }
}

void hash_chain_delete_by_letter(hash_table_chain_t* ht, char letter, int* deleted_count) {
    if (!ht) return;
    
    *deleted_count = 0;
    
    for (int i = 0; i < ht->size; i++) {
        hash_node_chain_t* current = ht->table[i];
        hash_node_chain_t* prev = NULL;
        
        while (current) {
            if (current->word[0] == letter) {
                (*deleted_count)++;
                
                hash_node_chain_t* to_delete = current;
                if (prev) {
                    prev->next = current->next;
                    current = current->next;
                } else {
                    ht->table[i] = current->next;
                    current = ht->table[i];
                }
                
                free(to_delete);
                ht->count--;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
}

void hash_chain_print(hash_table_chain_t* ht) {
    if (!ht) {
        printf("Хеш-таблица пуста\n");
        return;
    }
    
    printf("\n=== ХЕШ-ТАБЛИЦА (МЕТОД ЦЕПОЧЕК) ===\n");
    printf("Размер: %d, Элементов: %d, Коллизии: %d\n\n", 
           ht->size, ht->count, ht->collisions);
    
    for (int i = 0; i < ht->size; i++) {
        printf("[%3d]: ", i);
        
        hash_node_chain_t* current = ht->table[i];
        if (!current) {
            printf("(пусто)\n");
            continue;
        }
        
        while (current) {
            printf("%s", current->word);
            if (current->next) printf(" -> ");
            current = current->next;
        }
        printf("\n");
    }
    
    printf("\nКоэффициент заполнения: %.2f%%\n", 
           (float)ht->count / ht->size * 100);
    printf("Средняя длина цепочки: %.2f\n", 
           ht->collisions > 0 ? (float)(ht->count + ht->collisions) / ht->size : 0);
}

void hash_chain_free(hash_table_chain_t* ht) {
    if (!ht) return;
    
    for (int i = 0; i < ht->size; i++) {
        hash_node_chain_t* current = ht->table[i];
        while (current) {
            hash_node_chain_t* next = current->next;
            free(current);
            current = next;
        }
    }
    
    free(ht->table);
    free(ht);
}

hash_table_chain_t* hash_chain_read_from_file(const char* filename, int table_size) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Ошибка открытия файла: %s\n", filename);
        return NULL;
    }
    
    hash_table_chain_t* ht = hash_chain_create(table_size);
    if (!ht) {
        fclose(fp);
        return NULL;
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
                
                hash_chain_insert(ht, word);
                word_count++;
            }
            token = strtok(NULL, WORD_DELIMITERS);
        }
    }
    
    fclose(fp);
    
    printf("Загружено %d слов в хеш-таблицу (цепочки) из файла %s\n", 
           word_count, filename);
    
    return ht;
}

hash_table_open_t* hash_open_create(int size) {
    hash_table_open_t* ht = malloc(sizeof(hash_table_open_t));
    if (!ht) return NULL;
    
    ht->size = size;
    ht->count = 0;
    ht->collisions = 0;
    ht->table = calloc(size, sizeof(hash_node_open_t));
    
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    
    return ht;
}

int linear_probe(hash_table_open_t* ht, const char* word, int* comparisons) {
    int index = hash_function(word, ht->size);
    int original_index = index;
    
    while (ht->table[index].is_occupied) {
        (*comparisons)++;
        if (strcmp(ht->table[index].word, word) == 0) {
            return index;
        }
        
        index = (index + 1) % ht->size;
        if (index == original_index) {
            return -1;
        }
    }
    
    return index;
}

void hash_open_insert(hash_table_open_t* ht, const char* word) {
    if (!ht || ht->count >= ht->size) return;
    
    int comparisons = 0;
    int index = linear_probe(ht, word, &comparisons);
    
    if (index == -1) return;
    
    if (comparisons > 0) {
        ht->collisions++;
    }
    
    strcpy(ht->table[index].word, word);
    ht->table[index].is_occupied = 1;
    ht->count++;
}

int hash_open_search(hash_table_open_t* ht, const char* word, int* comparisons) {
    if (!ht) return 0;
    
    int index = hash_function(word, ht->size);
    int original_index = index;
    *comparisons = 0;
    
    while (ht->table[index].is_occupied) {
        (*comparisons)++;
        if (strcmp(ht->table[index].word, word) == 0) {
            return 1;
        }
        index = (index + 1) % ht->size;
        if (index == original_index) {
            break;
        }
    }
    
    return 0;
}

void hash_open_delete(hash_table_open_t* ht, const char* word) {
    if (!ht) return;
    
    int index = hash_function(word, ht->size);
    int original_index = index;
    
    while (ht->table[index].is_occupied) {
        if (strcmp(ht->table[index].word, word) == 0) {
            ht->table[index].is_occupied = 0;
            ht->count--;
            return;
        }
        
        index = (index + 1) % ht->size;
        if (index == original_index) {
            break;
        }
    }
}

void hash_open_delete_by_letter(hash_table_open_t* ht, char letter, int* deleted_count) {
    if (!ht) return;
    
    *deleted_count = 0;
    
    for (int i = 0; i < ht->size; i++) {
        if (ht->table[i].is_occupied && ht->table[i].word[0] == letter) {
            ht->table[i].is_occupied = 0;
            (*deleted_count)++;
            ht->count--;
        }
    }
}

void hash_open_print(hash_table_open_t* ht) {
    if (!ht) {
        printf("Хеш-таблица пуста\n");
        return;
    }
    
    printf("\n=== ХЕШ-ТАБЛИЦА (ОТКРЫТАЯ АДРЕСАЦИЯ) ===\n");
    printf("Размер: %d, Элементов: %d, Коллизии: %d\n\n", 
           ht->size, ht->count, ht->collisions);
    
    for (int i = 0; i < ht->size; i++) {
        printf("[%3d]: ", i);
        
        if (ht->table[i].is_occupied) {
            printf("%s\n", ht->table[i].word);
        } else {
            printf("(свободно)\n");
        }
    }
    
    printf("\nКоэффициент заполнения: %.2f%%\n", 
           (float)ht->count / ht->size * 100);
}

void hash_open_free(hash_table_open_t* ht) {
    if (!ht) return;
    
    free(ht->table);
    free(ht);
}

hash_table_open_t* hash_open_read_from_file(const char* filename, int table_size) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Ошибка открытия файла: %s\n", filename);
        return NULL;
    }
    
    hash_table_open_t* ht = hash_open_create(table_size);
    if (!ht) {
        fclose(fp);
        return NULL;
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
                
                hash_open_insert(ht, word);
                word_count++;
            }
            token = strtok(NULL, WORD_DELIMITERS);
        }
    }
    
    fclose(fp);
    
    printf("Загружено %d слов в хеш-таблицу (открытая адресация) из файла %s\n", 
           word_count, filename);
    
    return ht;
}

void print_hash_statistics(hash_table_chain_t* chain_ht, hash_table_open_t* open_ht) {
    printf("\nСТАТИСТИКА ХЕШ-ТАБЛИЦ\n");
    
    if (chain_ht) {
        printf("Метод цепочек:\n");
        printf("  Размер таблицы: %d\n", chain_ht->size);
        printf("  Количество элементов: %d\n", chain_ht->count);
        printf("  Коэффициент заполнения: %.2f%%\n", 
               (float)chain_ht->count / chain_ht->size * 100);
        printf("  Коллизии: %d\n", chain_ht->collisions);
        printf("  Средняя длина цепочки: %.2f\n", 
               chain_ht->count > 0 ? (float)(chain_ht->count + chain_ht->collisions) / chain_ht->size : 0);
    }
    
    if (open_ht) {
        printf("\nОткрытая адресация:\n");
        printf("  Размер таблицы: %d\n", open_ht->size);
        printf("  Количество элементов: %d\n", open_ht->count);
        printf("  Коэффициент заполнения: %.2f%%\n", 
               (float)open_ht->count / open_ht->size * 100);
        printf("  Коллизии: %d\n", open_ht->collisions);
    }
}
int hash_needs_rehash(hash_table_chain_t* ht, int max_comparisons) {
    if (!ht) return 0;
    
    float load_factor = (float)ht->count / ht->size;
    if (load_factor > MAX_LOAD_FACTOR) return 1;
    int max_chain_length = 0;
    for (int i = 0; i < ht->size; i++) {
        int chain_length = 0;
        hash_node_chain_t* current = ht->table[i];
        while (current) {
            chain_length++;
            current = current->next;
        }
        if (chain_length > max_chain_length) {
            max_chain_length = chain_length;
        }
    }
    
    return max_chain_length > max_comparisons;
}

int hash_open_needs_rehash(hash_table_open_t* ht, int max_comparisons) {
    if (!ht) return 0;
    float load_factor = (float)ht->count / ht->size;
    return load_factor > MAX_LOAD_FACTOR;
}
void hash_chain_rehash(hash_table_chain_t** ht_ptr) {
    if (!ht_ptr || !*ht_ptr) return;
    
    hash_table_chain_t* old_ht = *ht_ptr;
    int new_size = old_ht->size * 2 + 1; 
    hash_table_chain_t* new_ht = hash_chain_create(new_size);
    if (!new_ht) return;
    for (int i = 0; i < old_ht->size; i++) {
        hash_node_chain_t* current = old_ht->table[i];
        while (current) {
            hash_chain_insert(new_ht, current->word);
            current = current->next;
        }
    }
    hash_chain_free(old_ht);
    *ht_ptr = new_ht;
    
    printf("Таблица перехеширована. Новый размер: %d\n", new_size);
}
void hash_open_rehash(hash_table_open_t** ht_ptr) {
    if (!ht_ptr || !*ht_ptr) return;
    
    hash_table_open_t* old_ht = *ht_ptr;
    int new_size = old_ht->size * 2 + 1;
    hash_table_open_t* new_ht = hash_open_create(new_size);
    if (!new_ht) return;
    for (int i = 0; i < old_ht->size; i++) {
        if (old_ht->table[i].is_occupied) {
            hash_open_insert(new_ht, old_ht->table[i].word);
        }
    }
    hash_open_free(old_ht);
    *ht_ptr = new_ht;
    
    printf("Таблица перехеширована. Новый размер: %d\n", new_size);
}