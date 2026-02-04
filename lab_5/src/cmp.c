#include "lib.h"

extern int show_memory_operations;

static void clean_stdin(void)
{
    char c;
    while ((c = getchar()) != '\n');
}

static int read_str(char *str_dst, size_t max_len)
{
    char *tmp = malloc((max_len + 2) * sizeof(char));
    if (!tmp)
        return STR_LEN_ERROR;
        
    if (fgets(tmp, max_len + 2, stdin) == NULL)
    {
        free(tmp);
        return EMPTY_INPUT_ERROR;
    }

    if (tmp[strlen(tmp) - 1] == '\n')
        tmp[strlen(tmp) - 1] = '\0';
    else
    {
        clean_stdin();
        free(tmp);
        return STR_LEN_ERROR;
    }

    if (strlen(tmp) == 0)
    {
        free(tmp);
        return STR_LEN_ERROR;
    }

    if (strlen(tmp) > max_len)
    {
        free(tmp);
        return STR_LEN_ERROR;
    }

    strcpy(str_dst, tmp);
    free(tmp);
    return EXIT_SUCCESS;
}

static int is_int(const char *s)
{
    for (size_t i = 0; i < strlen(s); i++)
    {
        if (!isdigit(s[i]))
        {
            if (i == 0 && (s[i] == '-' || s[i] == '+'))
                continue;
            return 0;
        }
    }
    return 1;
}

int read_int(int *num)
{
    char tmp[MAX_STR_LEN + 1];
    int max_len = MAX_STR_LEN;
    int rc = read_str(tmp, max_len);
    if (rc != EXIT_SUCCESS)
        return rc;
    if (!is_int(tmp))
        return NOT_NUMBER_ERROR;
    if (sscanf(tmp, "%d", num) != 1)
        return NOT_NUMBER_ERROR;
    return EXIT_SUCCESS;
}

static int is_double(char *s)
{
    int is_dot = 0;
    for (size_t i = 0; i < strlen(s); i++)
    {
        if (!isdigit(s[i]))
        {
            if (i == 0 && (s[i] == '-' || s[i] == '+'))
                continue;
            else if (s[i] == '.' && !is_dot && i > 0)
            {
                is_dot = 1;
                continue;
            }
            else
                return 0;
        }
    }
    return 1;
}

int read_double(double *num)
{
    char tmp[MAX_STR_LEN + 1];
    int max_len = MAX_STR_LEN;
    int rc = read_str(tmp, max_len);
    if (rc != EXIT_SUCCESS)
        return rc;
    if (!is_double(tmp))
        return NOT_NUMBER_ERROR;
    if (sscanf(tmp, "%lf", num) != 1)
        return NOT_NUMBER_ERROR;
    return EXIT_SUCCESS;
}

unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;

    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

static double calculate_theoretical_time(int oa2_activations, int oa1_requests, double t1_start, double t1_end,
                                         double t2_start, double t2_end)
{
    double avg_t1 = (t1_start + t1_end) / 2.0;
    double avg_t2 = (t2_start + t2_end) / 2.0;

    double time_oa1 = avg_t1 * oa1_requests;
    double time_oa2 = avg_t2 * oa2_activations;

    return (time_oa1 > time_oa2) ? time_oa1 : time_oa2;
}

void simulation_queue(queue_t *queue1, queue_t *queue2,
                      double t1_start, double t1_end,
                      double t2_start, double t2_end,
                      double probability)
{

    service_unit_t oa1 = { .is_busy = 0, .end_time = 0.0 };
    service_unit_t oa2 = { .is_busy = 0, .end_time = 0.0 };

    double current_time = 0.0;
    int processed_requests = 0;
    int oa1_activations = 0;
    double total_oa2_idle_time = 0.0;
    double queue_stats[2] = { 0.0, 0.0 };
    int stat_points = 0;
    for (int i = 0; i < 100; i++)
    {
        request_t req = { i, current_time };
        enqueue(queue1, req);
    }

    while (processed_requests < 1000)
    {
        if (!oa1.is_busy && !is_queue_empty(queue1))
        {
            request_t req;
            dequeue(queue1, &req);
            oa1.is_busy = 1;
            oa1.current_request = req;
            double service_time = t1_start + ((double) rand() / RAND_MAX) * (t1_end - t1_start);
            oa1.end_time = current_time + service_time;
            oa1_activations++;
        }
        if (!oa2.is_busy && !is_queue_empty(queue2))
        {
            request_t req;
            dequeue(queue2, &req);
            oa2.is_busy = 1;
            oa2.current_request = req;
            double service_time = t2_start + ((double) rand() / RAND_MAX) * (t2_end - t2_start);
            oa2.end_time = current_time + service_time;
        }
        double next_time = 0.0;
        int next_unit = 0; 

        if (oa1.is_busy && oa2.is_busy)
        {
            if (oa1.end_time <= oa2.end_time)
            {
                next_time = oa1.end_time;
                next_unit = 1;
            }
            else
            {
                next_time = oa2.end_time;
                next_unit = 2;
            }
        }
        else if (oa1.is_busy)
        {
            next_time = oa1.end_time;
            next_unit = 1;
        }
        else if (oa2.is_busy)
        {
            next_time = oa2.end_time;
            next_unit = 2;
        }

        if (!oa2.is_busy)
            total_oa2_idle_time += (next_time - current_time);
        current_time = next_time;

        if (next_unit == 1)
        {
            if (((double) rand() / RAND_MAX) < probability)
                enqueue(queue1, oa1.current_request);
            else
                enqueue(queue2, oa1.current_request);
            oa1.is_busy = 0;
        }
        else
        {
            enqueue(queue1, oa2.current_request);
            oa2.is_busy = 0;
            processed_requests++;

            if (processed_requests % 100 == 0)
            {
                queue_stats[0] += queue_length(queue1);
                queue_stats[1] += queue_length(queue2);
                stat_points++;
                printf("Обработано заявок во втором аппарате: %d\n", processed_requests);
                printf("Текущая длины очереди: Q1 = %d, Q2 = %d\n",
                       queue_length(queue1), queue_length(queue2));
                printf("Средние длины очередей: Q1 = %.2f, Q2= %.2f\n",
                       queue_stats[0] / stat_points,
                       queue_stats[1] / stat_points);
            }
        }
    }
    double theoretical_time = calculate_theoretical_time(1000, oa1_activations,
                                                         t1_start, t1_end,
                                                         t2_start, t2_end);

    double time_difference = fabs(current_time - theoretical_time);
    double difference_percent = (time_difference / theoretical_time) * 100.0;

    printf("\nИтоги симуляции:\n");
    printf("Общее время работы: %.2f\n", current_time);
    printf("Теоретический рассчёт: %.2f\n", theoretical_time);
    printf("Процент погрешности: %.2f%%\n", difference_percent);
    printf("Время простоя ОА2: %.2f\n", total_oa2_idle_time);
    printf("Количество срабатываний ОА1: %d\n", oa1_activations);
    printf("Средняя длина первой очереди %.2f\n", queue_stats[0] / stat_points);
    printf("Средняя длина второй очереди: %.2f\n", queue_stats[1] / stat_points);

}

void queue_init(queue_t *queue, queue_type_t queue_type, int max_size)
{
    queue->type = queue_type;
    if (queue_type == ARRAY_QUEUE)
        arr_queue_init(&(queue->arr_queue), max_size);
    else if (queue_type == LIST_QUEUE)
        list_queue_init(&(queue->list_queue), max_size);
}

int is_queue_empty(queue_t *queue)
{
    if (queue->type == ARRAY_QUEUE)
        return is_arr_queue_empty(&(queue->arr_queue));
    return is_list_queue_empty(&(queue->list_queue));
}

int is_queue_full(queue_t *queue)
{
    if (queue->type == ARRAY_QUEUE)
        return is_arr_queue_full(&(queue->arr_queue));
    return is_list_queue_full(&(queue->list_queue));
}

int enqueue(queue_t *queue, request_t elem)
{
    if (is_queue_full(queue))
        return QUEUE_OVERFLOW;
    if (queue->type == ARRAY_QUEUE)
        arr_enqueue(&(queue->arr_queue), elem);
    else if (queue->type == LIST_QUEUE)
    {
        int rc = list_enqueue(&(queue->list_queue), elem);
        if (rc != EXIT_SUCCESS)
            return rc;
    }

    return EXIT_SUCCESS;
}

int dequeue(queue_t *queue, request_t *removed_elem)
{
    if (is_queue_empty(queue))
        return QUEUE_UNDERFLOW;

    if (queue->type == ARRAY_QUEUE)
        *removed_elem = arr_dequeque(&(queue->arr_queue));
    else if (queue->type == LIST_QUEUE)
    {
        int rc = list_dequeue(&(queue->list_queue), removed_elem);
        if (rc != EXIT_SUCCESS)
            return rc;
    }
    return EXIT_SUCCESS;
}

int queue_length(queue_t *queue)
{
    if (queue->type == ARRAY_QUEUE)
        return queue->arr_queue.size;
    return queue->list_queue.size;
}

void queue_free(queue_t *queue)
{
    if (queue->type == LIST_QUEUE)
        free_list_queue(&(queue->list_queue));
}