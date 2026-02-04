#ifndef KEYS_OPS_H
#define KEYS_OPS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "io_struct.h"
#define MAX_LEN 100
#define AGE 4
#define MAX_TABLE_SIZE 500
#define NOT_FOUND -1
struct KeyRecord {
    char key_value[MAX_LEN];
    int original_index;
};
void create_key_table(struct Spectacle *spectacle_table, int spectacle_count, struct KeyRecord *key_table, int *key_count);
void print_key_table(int key_count ,struct KeyRecord *key_table);
void print_table_by_keys(struct Spectacle *spectacle_table, struct KeyRecord *key_table, int key_count);
#endif