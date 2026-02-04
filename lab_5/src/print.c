#include "lib.h"

void menu_print(void)
{
    printf("\n");
    printf("Программа для работы с очередью.\n");
    printf("1) Провести симуляцию процесса обслуживания очереди на массиве\n");
    printf("2) Провести симуляцию процесса обслуживания очереди на списке\n");
    printf("3) Изменить параметры симуляции\n");
    printf("4) Вывести результат замеров удаления и добавления\n");
    printf("0) Завершить работу программы\n");
    printf("\n");
    printf("Введите пункт: ");
}

void print_queue(queue_t *queue)
{
    if (queue->type == ARRAY_QUEUE)
        print_arr_queue(&(queue->arr_queue));
    else if (queue->type == LIST_QUEUE)
        print_list_queue(&(queue->list_queue));
}

void queue_print_free_area(queue_t *queue)
{
    if (queue->type == LIST_QUEUE)
        print_free_area(&(queue->list_queue));
}

static void print_table_header(void)
{
    printf("| %-15s| %-16s| %-16s| %-16s| %-16s| %-16s| %-16s|\n", "Queue length",
           "Arr queue push", "List queue push", "Arr queue pop", "List queue pop",
           "Arr queue memory", "List queue memory");
    printf("|----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|\n");
}

static double arr_stack_push_time(arr_queue_t *queue, request_t seq[], int seq_len, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        arr_queue_init(queue, seq_len);
        beg = microseconds_now();
        for (int c = 0; c < seq_len; c++)
            arr_enqueue(queue, seq[c]);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double list_stack_push_time(list_queue_t *queue, request_t seq[], int seq_len, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        list_queue_init(queue, seq_len);
        beg = microseconds_now();
        for (int c = 0; c < seq_len; c++)
            list_enqueue(queue, seq[c]);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double arr_stack_pop_time(arr_queue_t *queue, request_t seq[], int seq_len, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        arr_queue_init(queue, seq_len);
        for (int c = 0; c < seq_len; c++)
            arr_enqueue(queue, seq[c]);
        beg = microseconds_now();
        for (int c = 0; c < seq_len; c++)
            arr_dequeque(queue);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double list_stack_pop_time(list_queue_t *queue, request_t seq[], int seq_len, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    request_t tmp;
    for (int i = 0; i < count; i++)
    {
        list_queue_init(queue, seq_len);
        for (int c = 0; c < seq_len; c++)
            list_enqueue(queue, seq[c]);
        beg = microseconds_now();
        for (int c = 0; c < seq_len; c++)
            list_dequeue(queue, &tmp);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static void print_time(int seq_len, request_t seq[])
{
    list_queue_t list_queue;
    list_queue_init(&list_queue, seq_len);

    arr_queue_t arr_queue;
    arr_queue_init(&arr_queue, seq_len);

    size_t arr_queue_memory = sizeof(request_t) * MAX_ARR_SIZE;
    size_t list_queue_memory = sizeof(node_t) * seq_len;
    double time_arr_push = arr_stack_push_time(&arr_queue, seq, seq_len, NUM_OF_ITERATIONS);
    double time_list_push = list_stack_push_time(&list_queue, seq, seq_len, NUM_OF_ITERATIONS);
    double time_arr_pop = arr_stack_pop_time(&arr_queue, seq, seq_len, NUM_OF_ITERATIONS);
    double time_list_pop = list_stack_pop_time(&list_queue, seq, seq_len, NUM_OF_ITERATIONS);

    printf("| %-15d| %-16lf| %-16lf| %-16lf| %-16lf| %-16zu| %-16zu|\n", seq_len,
           time_arr_push, time_list_push, time_arr_pop, time_list_pop, arr_queue_memory, list_queue_memory);
    printf("|----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|\n");

    free_list_queue(&list_queue);
}

static void fill_arr(request_t arr[], int len)
{
    srand(time(NULL));
    for (int i = 0; i < len; i++)
    {
        arr[i].id = rand() % 100;
        arr[i].entered_time = 0.0;
    }
}

void print_measurements(void)
{
    int seq_lenghts[] = { 1, 10, 50, 100, 335, 500 };

    print_table_header();
    for (size_t i = 0; i < 6; i++)
    {
        request_t *seq = malloc(seq_lenghts[i] * sizeof(request_t));
        if (seq)
        {
            fill_arr(seq, seq_lenghts[i]);
            print_time(seq_lenghts[i], seq);
            free(seq);
        }
    }
    printf("\n");
}