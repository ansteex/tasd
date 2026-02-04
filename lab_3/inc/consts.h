#ifndef CONST
#define CONST
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define SUCCESS 0
#define INPUT_ERROR 1
#define MAX_DIMENSION 1000
#define INITIAL_CAPACITY 10
#define MULT_ERROR 2

struct CompactCSR {
    int *elements;        
    int *column_indexes;   
    int *row_starts;  
    int num_rows;
    int num_cols;
    int element_count;          
    int buffer_size;      
};

struct CompactCSC {
    int *elements;        
    int *row_indexes;   
    int *col_starts;  
    int num_rows;
    int num_cols;
    int element_count;         
    int buffer_size;
};

struct StandardMat {
    int **matrix;
    int num_rows;
    int num_cols;
};

#endif
