#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define ERROR 1
#define MULT_ERROR 1
#define MAX_LEN_MANT 30
#define MAX_EXPONENT 5
#define MAX_RESULT_MANT 40
#define MAX_INT_MANT 40
#define MAX_EXSP 99999
#define MAX_LEN 50
#define INP_ERROR 2
#define PRINT_ERROR 3
struct short_num {
    char sign;
    char mantissa[MAX_LEN_MANT + 1];
    int exponent;
    int len_m;
};

struct long_num {
    char sign;
    char mantissa[MAX_INT_MANT + 1];
    int exponent;
    int len_m;
};
int parse_integer(const char* str, struct long_num* result);
int parse_float(const char* str, struct short_num* result);
int multiply_numbers(const struct long_num* a, const struct short_num* b, struct long_num* result);
int print_normalized(const struct long_num* num);

bool test_parse_integer();
bool test_parse_float();
bool test_multiply_numbers();
bool test_print_normalized();
bool run_all_tests();
#endif