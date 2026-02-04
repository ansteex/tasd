#ifndef SORT_H
#define SORT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "io_struct.h"
#include "keys_ops.h"
#define MAX_LEN 100
#define AGE 4
#define MAX_TABLE_SIZE 500
#define NOT_FOUND -1

void bubble_sort_spectacles(struct Spectacle *spectacle_table,int spectacle_count);
void bubble_sort_keys(struct KeyRecord *key_table, int key_count);
void quick_sort_spectacles(int left, int right,struct Spectacle *spectacle_table,int spectacle_count);
void quick_sort_keys(int left, int right,struct KeyRecord *key_table, int key_count);
double bubble_sort_spectacles_with_time(struct Spectacle *spectacle_table, int spectacle_count);
double bubble_sort_keys_with_time(struct KeyRecord *key_table, int key_count);
double quick_sort_spectacles_with_time(struct Spectacle *spectacle_table, int spectacle_count);
double quick_sort_keys_with_time(struct KeyRecord *key_table, int key_count);
double measure_sorting_time(void (*sort_func)(void*, int), void *array, int count, int is_quick_sort, int array_type);
void compare_efficiency(struct Spectacle *spectacle_table, int spectacle_count, 
                       struct KeyRecord *key_table, int key_count);
#endif