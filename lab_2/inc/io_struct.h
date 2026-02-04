#ifndef IO_STUCT_H
#define IO_STUCT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LEN 100
#define AGE 4
#define MAX_TABLE_SIZE 500
#define ERROR 1
enum AGE_LIMIT {AGE_3, AGE_10, AGE_16, AGE_ERROR};
struct Play {
    int play_type;
    enum AGE_LIMIT age;
};

struct Musical {
    char composer[MAX_LEN];
    char country[MAX_LEN];
    int musical_type;
    enum AGE_LIMIT age; 
    int duration;
};
struct Spectacle {
    char theatre_name[MAX_LEN];
    char spec_name[MAX_LEN];
    int min_price;
    int max_price;
    int type;
    
    union {
        struct Play play; 
        struct Musical musical; 
    };
};
void clear_input_buffer();
const char* get_spectacle_type_name(int type);
const char* get_play_type_name(int play_type);
const char* get_musical_type_name(int musical_type);
enum AGE_LIMIT age_limit_from_string(char* s);
void input_spectacle(struct Spectacle *s);
void print_spectacle(struct Spectacle *s, int index);
#endif