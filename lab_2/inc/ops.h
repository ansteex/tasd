#ifndef OPS_H
#define OPS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "io_struct.h"
#include "read_from_file.h"
#define MAX_LEN 100
#define AGE 4
#define MAX_TABLE_SIZE 500
#define NOT_FOUND -1
void print_table(struct Spectacle *spectacle_table,int spectacle_count);
void add_spectacle(struct Spectacle *spectacle_table,int *spectacle_count) ;
void delete_spectacle(int index,struct Spectacle *spectacle_table,int *spectacle_count);
int find_spectacle_by_name(char *name,struct Spectacle *spectacle_table,int spectacle_count);
void find_ballet(struct Spectacle *spectacle_table,int spectacle_count);
void generate_test_data(struct Spectacle *spectacle_table,int *spectacle_count);
#endif