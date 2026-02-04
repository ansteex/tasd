#ifndef READ_FROM_FILE_H
#define READ_FROM_FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "io_struct.h"
#define MAX_LEN 100
#define AGE 4
#define MAX_TABLE_SIZE 500
#define ERROR 1
int input_spectacle_from_file(FILE *f, struct Spectacle *s);
#endif