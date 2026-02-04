#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define MAX_ARR_SIZE 500
#define MAX_STR_LEN 30
#define NUM_OF_ITERATIONS 10000


typedef struct
{
    int id;
    double entered_time;
} request_t;

typedef struct
{
    request_t data[MAX_ARR_SIZE];
    int size;
    int max_size;
    int head;
    int tail;
} arr_queue_t;

typedef struct node
{
    request_t data;
    struct node *next;
} node_t;

typedef struct free_node
{
    void *address;
    struct free_node *next;
} free_node_t;

typedef struct
{
    node_t *head;
    node_t *tail;
    int size;
    int max_size;
    free_node_t *free_list;
    int free_elems_size;
} list_queue_t;

typedef enum {
    ARRAY_QUEUE,
    LIST_QUEUE
} queue_type_t;

typedef struct {
    queue_type_t type;
    arr_queue_t arr_queue;
    list_queue_t list_queue;
} queue_t;

typedef struct
{
    int is_busy;
    double end_time;
    request_t current_request;
} service_unit_t;


typedef enum
{
    EXIT,
    MAKE_SIMULATION_ON_ARR,
    MAKE_SIMULATION_ON_LIST,
    CHANGE_PARAMETRS,
    PRINT_MEASUREMENTS
} menu_t;

enum errors
{
    QUEUE_OVERFLOW = 1,
    QUEUE_UNDERFLOW,
    NODE_ALLOC_ERROR = 3,
    FREE_NODE_ALLOC_ERROR,
    NOT_NUMBER_ERROR = 6,
    EMPTY_INPUT_ERROR,
    STR_LEN_ERROR,
    WRONG_VALUE_READ_ERROR = 11
};

extern int show_memory_operations;


void arr_queue_init(arr_queue_t *queue, int max_size);
int is_arr_queue_empty(arr_queue_t *queue);
int is_arr_queue_full(arr_queue_t *queue);
void arr_enqueue(arr_queue_t *queue, request_t elem);
request_t arr_dequeque(arr_queue_t *queue);
void print_arr_queue(arr_queue_t *queue);

void list_queue_init(list_queue_t *queue, int max_size);
int is_list_queue_empty(list_queue_t *queue);
int is_list_queue_full(list_queue_t *queue);
int list_enqueue(list_queue_t *queue, request_t elem);
int list_dequeue(list_queue_t *queue, request_t *removed_elem);
void print_list_queue(list_queue_t *queue);
void free_list_queue(list_queue_t *queue);
void print_free_area(list_queue_t *queue);
void test_memory_fragmentation(int initial_fill, int remove_count, int additional_fill);


int read_int(int *num);
int read_double(double *num);
void print_measurements(void);
unsigned long long microseconds_now(void);


void menu_print(void);
void print_queue(queue_t *queue);
void queue_print_free_area(queue_t *queue);

void queue_init(queue_t *queue, queue_type_t queue_type, int max_size);
int is_queue_empty(queue_t *queue);
int is_queue_full(queue_t *queue);
int enqueue(queue_t *queue, request_t elem);
int dequeue(queue_t *queue, request_t *removed_elem);
int queue_length(queue_t *queue);
void queue_free(queue_t *queue);
void simulation_queue(queue_t *queue1, queue_t *queue2, double t1_start, double t1_end,
                      double t2_start, double t2_end, double probability);


void print_arr_queue(arr_queue_t *queue);
void print_list_queue(list_queue_t *queue);
void print_free_area(list_queue_t *queue);

#endif