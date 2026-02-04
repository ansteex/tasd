#include "funcs.h"

int initialize_csr(struct CompactCSR *csr, int num_rows, int num_cols, int buffer_size) 
{
    csr->elements = malloc(buffer_size * sizeof(int));
    csr->column_indexes = malloc(buffer_size * sizeof(int));
    csr->row_starts = calloc(num_rows + 1, sizeof(int));
    
    if (!csr->elements || !csr->column_indexes || !csr->row_starts) 
    {
        free(csr->elements);
        free(csr->column_indexes);
        free(csr->row_starts);
        return INPUT_ERROR;
    }
    
    csr->element_count = 0;
    csr->buffer_size = buffer_size;
    csr->num_rows = num_rows;
    csr->num_cols = num_cols;
    
    return SUCCESS;
}

int insert_element_csr(struct CompactCSR *csr, int row, int col, int val) 
{
    if (val == 0) 
        return SUCCESS;
    
    if (row < 0 || row >= csr->num_rows || col < 0 || col >= csr->num_cols) 
        return INPUT_ERROR;
    
    if (csr->element_count >= csr->buffer_size) 
    {
        int new_buffer = csr->buffer_size * 2;
        int *new_elements = realloc(csr->elements, new_buffer * sizeof(int));
        int *new_col_indexes = realloc(csr->column_indexes, new_buffer * sizeof(int));
        
        if (!new_elements || !new_col_indexes) 
        {
            free(new_elements);
            free(new_col_indexes);
            return INPUT_ERROR;
        }
        
        csr->elements = new_elements;
        csr->column_indexes = new_col_indexes;
        csr->buffer_size = new_buffer;
    }
    
    csr->elements[csr->element_count] = val;
    csr->column_indexes[csr->element_count] = col;
    csr->element_count++;
    
    csr->row_starts[row + 1]++;
    
    return SUCCESS;
}

int finalize_csr_structure(struct CompactCSR *csr) 
{
    for (int i = 1; i <= csr->num_rows; i++) 
        csr->row_starts[i] += csr->row_starts[i - 1];
    return SUCCESS;
}

int read_coordinate_matrix(struct StandardMat *mat) 
{
    printf("Координатный ввод матрицы %dx%d\n", mat->num_rows, mat->num_cols);
    printf("Формат ввода для каждого элемента:\n");
    printf("номер_строки\nномер_столбца\nзначение\n");
    printf("Индексы начинаются с 1\n");
    printf("Для завершения введите -1 для номера строки\n\n");
    
    for (int i = 0; i < mat->num_rows; i++) 
    {
        for (int j = 0; j < mat->num_cols; j++)
            mat->matrix[i][j] = 0;
    }
    
    int row_idx, col_idx, element_val;
    int counter = 0;
    int found_non_zero = 0;
    char input_line[100];
    
    while (1) 
    {
        printf("Элемент %d:\n", counter + 1);
        
        printf("Строка: ");
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) 
        {
            printf("Ошибка чтения ввода\n");
            return INPUT_ERROR;
        }
        
        if (sscanf(input_line, "%d", &row_idx) != 1) 
        {
            printf("Ошибка: требуется целое число\n");
            continue;
        }
        
        char extra_char;
        if (sscanf(input_line, "%d %c", &row_idx, &extra_char) == 2) 
        {
            printf("Ошибка: ввод должен содержать только число\n");
            continue;
        }
        
        if (row_idx == -1) 
        {
            if (counter == 0) 
            {
                printf("Ошибка: необходимо ввести хотя бы один ненулевой элемент!\n");
                continue;
            }
            break;
        }
        
        printf("Столбец: ");
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) 
        {
            printf("Ошибка чтения ввода\n");
            return INPUT_ERROR;
        }
        
        if (sscanf(input_line, "%d", &col_idx) != 1) 
        {
            printf("Ошибка: требуется целое число\n");
            continue;
        }
        
        if (sscanf(input_line, "%d %c", &col_idx, &extra_char) == 2) 
        {
            printf("Ошибка: ввод должен содержать только число\n");
            continue;
        }
        
        printf("Значение: ");
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) 
        {
            printf("Ошибка чтения ввода\n");
            return INPUT_ERROR;
        }
        
        if (sscanf(input_line, "%d", &element_val) != 1) 
        {
            printf("Ошибка: требуется целое число\n");
            continue;
        }
        
        if (sscanf(input_line, "%d %c", &element_val, &extra_char) == 2) 
        {
            printf("Ошибка: ввод должен содержать только число\n");
            continue;
        }
        if (row_idx < 1 || row_idx > mat->num_rows || col_idx < 1 || col_idx > mat->num_cols) 
        {
            printf("Ошибка: координаты (%d, %d) вне диапазона матрицы %dx%d (индексы от 1 до %d и от 1 до %d)\n", 
                   row_idx, col_idx, mat->num_rows, mat->num_cols, mat->num_rows, mat->num_cols);
            continue;
        }
        
        if (element_val != 0) found_non_zero = 1;
        mat->matrix[row_idx - 1][col_idx - 1] = element_val;  
        counter++;
        printf("Добавлен элемент: (%d, %d) = %d\n", row_idx, col_idx, element_val);
    }
    
    if (!found_non_zero) 
    {
        printf("Ошибка: матрица должна содержать минимум один ненулевой элемент\n");
        return INPUT_ERROR;
    }
    
    return SUCCESS;
}

void clean_input_buffer(void) 
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int read_standard_matrix(struct StandardMat *mat, int fixed_rows) 
{
    char input_line[100];
    
    if (fixed_rows > 0) 
    {
        mat->num_rows = fixed_rows;
        printf("Число строк автоматически установлено: %d\n", fixed_rows);
    } 
    else 
    {
        printf("Введите число строк: ");
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) 
        {
            printf("Ошибка чтения ввода\n");
            return INPUT_ERROR;
        }
        
        char extra_char;
        if (sscanf(input_line, "%d %c", &mat->num_rows, &extra_char) == 2) 
        {
            printf("Ошибка: введите одно число\n");
            return INPUT_ERROR;
        }
        
        if (sscanf(input_line, "%d", &mat->num_rows) != 1 || mat->num_rows <= 0 || mat->num_rows > MAX_DIMENSION) 
        {
            printf("Ошибка: некорректное число строк (максимум %d)\n", MAX_DIMENSION);
            return INPUT_ERROR;
        }
    }
    printf("Введите число столбцов: ");
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) 
    {
        printf("Ошибка чтения ввода\n");
        return INPUT_ERROR;
    }
    
    char extra_char;
    if (sscanf(input_line, "%d %c", &mat->num_cols, &extra_char) == 2) 
    {
        printf("Ошибка: введите одно число\n");
        return INPUT_ERROR;
    }
    
    if (sscanf(input_line, "%d", &mat->num_cols) != 1 || mat->num_cols <= 0 || mat->num_cols > MAX_DIMENSION) 
    {
        printf("Ошибка: некорректное число столбцов (максимум %d)\n", MAX_DIMENSION);
        return INPUT_ERROR;
    }

    mat->matrix = malloc(mat->num_rows * sizeof(int*));
    if (!mat->matrix) return INPUT_ERROR;
    
    for (int i = 0; i < mat->num_rows; i++) 
    {
        mat->matrix[i] = calloc(mat->num_cols, sizeof(int));
        if (!mat->matrix[i]) 
        {
            for (int j = 0; j < i; j++) free(mat->matrix[j]);
            free(mat->matrix);
            return INPUT_ERROR;
        }
    }
    
    printf("Выберите метод ввода для матрицы %dx%d:\n", mat->num_rows, mat->num_cols);
    printf("1 - Полный ввод (все элементы)\n");
    printf("2 - Координатный ввод (только ненулевые элементы, индексация с 1)\n");
    
    int selection;
    if (scanf("%d", &selection) != 1) 
    {
        clean_input_buffer();
        return INPUT_ERROR;
    }
    
    clean_input_buffer();
    
    if (selection == 1) 
    {
        printf("Полный ввод матрицы %dx%d:\n", mat->num_rows, mat->num_cols);
        printf("Введите элементы построчно (индексация с 1):\n");
        int has_non_zero = 0;
        for (int i = 0; i < mat->num_rows; i++) 
        {
            printf("Строка %d: ", i + 1);
            for (int j = 0; j < mat->num_cols; j++) 
            {
                if (scanf("%d", &mat->matrix[i][j]) != 1) 
                {
                    printf("Ошибка формата ввода. Требуется целое число.\n");
                    clean_input_buffer();
                    return INPUT_ERROR;
                }
                if (mat->matrix[i][j] != 0) 
                    has_non_zero = 1;
            }
        }
        if (!has_non_zero) 
        {
            printf("Ошибка: должен быть хотя бы один ненулевой элемент\n");
            return INPUT_ERROR;
        }
    } 
    else if (selection == 2) 
        return read_coordinate_matrix(mat);
    else 
    {
        printf("Неверный выбор\n");
        return INPUT_ERROR;
    }
    
    return SUCCESS;
}

int create_random_matrix(struct StandardMat *mat, int rows, int cols, int non_zero_count) 
{
    if (non_zero_count < 1 || non_zero_count > rows * cols) 
    {
        printf("Ошибка: число ненулевых элементов должно быть от 1 до %d\n", rows * cols);
        return INPUT_ERROR;
    }
    
    mat->num_rows = rows;
    mat->num_cols = cols;
    
    mat->matrix = malloc(mat->num_rows * sizeof(int*));
    if (!mat->matrix) return INPUT_ERROR;
    
    for (int i = 0; i < mat->num_rows; i++) 
    {
        mat->matrix[i] = calloc(mat->num_cols, sizeof(int));
        if (!mat->matrix[i]) 
        {
            for (int j = 0; j < i; j++) free(mat->matrix[j]);
            free(mat->matrix);
            return INPUT_ERROR;
        }
    }
    
    srand(time(NULL) + rand());
    int added_elements = 0;
    
    while (added_elements < non_zero_count) 
    {
        int row_idx = rand() % rows;
        int col_idx = rand() % cols;
        
        if (mat->matrix[row_idx][col_idx] == 0) 
        {
            mat->matrix[row_idx][col_idx] = rand() % 100 + 1;
            added_elements++;
        }
    }
    
    return SUCCESS;
}

void display_standard_matrix(const struct StandardMat *mat) 
{
    if (mat->num_rows > 30 || mat->num_cols > 30) 
    {
        printf("Матрица слишком большая для отображения (%dx%d)\n", mat->num_rows, mat->num_cols);
        return;
    }
    
    printf("Полная матрица %dx%d:\n", mat->num_rows, mat->num_cols);
    for (int i = 0; i < mat->num_rows; i++) 
    {
        for (int j = 0; j < mat->num_cols; j++) 
        {
            printf("%6d ", mat->matrix[i][j]);
        }
        printf("\n");
    }
}

void display_csr_format(const struct CompactCSR *csr) 
{
    printf("CSR представление матрицы %dx%d:\n", csr->num_rows, csr->num_cols);
    
    if (csr->element_count == 0) {
        printf("Матрица пуста\n");
        return;
    }
    
    printf("\nЭлементы (A): \n");
    for (int i = 0; i < csr->element_count; i++) 
    {
        printf("%d ", csr->elements[i]);
    }
    printf("\nНачала строк (IA): \n");
    for (int i = 0; i <= csr->num_rows; i++) 
    {
        printf("%d ", csr->row_starts[i]);
    }
    printf("\nИндексы столбцов (JA): \n");
    for (int i = 0; i < csr->element_count; i++) 
    {
        printf("%d ", csr->column_indexes[i]);
    }
}

void release_standard_matrix(struct StandardMat *mat) 
{
    if (mat->matrix) 
    {
        for (int i = 0; i < mat->num_rows; i++) 
            free(mat->matrix[i]);
        free(mat->matrix);
        mat->matrix = NULL;
    }
    mat->num_rows = 0;
    mat->num_cols = 0;
}

void release_csr_matrix(struct CompactCSR *mat) 
{
    if (mat->elements) 
    {
        free(mat->elements);
        mat->elements = NULL;
    }
    if (mat->column_indexes) 
    {
        free(mat->column_indexes);
        mat->column_indexes = NULL;
    }
    if (mat->row_starts) 
    {
        free(mat->row_starts);
        mat->row_starts = NULL;
    }
    mat->element_count = 0;
    mat->buffer_size = 0;
    mat->num_rows = 0;
    mat->num_cols = 0;
}

int convert_standard_to_csr(const struct StandardMat *std_mat, struct CompactCSR *csr) 
{
    int non_zero_elements = 0;
    
    for (int i = 0; i < std_mat->num_rows; i++) 
    {
        for (int j = 0; j < std_mat->num_cols; j++) 
        {
            if (std_mat->matrix[i][j] != 0)
                non_zero_elements++;
        }
    }
    
    if (initialize_csr(csr, std_mat->num_rows, std_mat->num_cols, non_zero_elements) != SUCCESS) 
        return INPUT_ERROR;
    
    for (int i = 0; i < std_mat->num_rows; i++) 
    {
        for (int j = 0; j < std_mat->num_cols; j++) 
        {
            if (std_mat->matrix[i][j] != 0) 
            {
                if (insert_element_csr(csr, i, j, std_mat->matrix[i][j]) != SUCCESS) 
                {
                    release_csr_matrix(csr);
                    return INPUT_ERROR;
                }
            }
        }
    }
    
    return finalize_csr_structure(csr);
}

int convert_csr_to_standard(const struct CompactCSR *csr, struct StandardMat *std_mat) 
{
    std_mat->num_rows = csr->num_rows;
    std_mat->num_cols = csr->num_cols;
    
    std_mat->matrix = malloc(std_mat->num_rows * sizeof(int*));
    if (!std_mat->matrix) return INPUT_ERROR;
    
    for (int i = 0; i < std_mat->num_rows; i++) 
    {
        std_mat->matrix[i] = calloc(std_mat->num_cols, sizeof(int));
        if (!std_mat->matrix[i]) 
        {
            for (int j = 0; j < i; j++) 
                free(std_mat->matrix[j]);
            free(std_mat->matrix);
            return INPUT_ERROR;
        }
    }
    
    for (int i = 0; i < csr->num_rows; i++) 
    {
        int start_idx = csr->row_starts[i];
        int end_idx = csr->row_starts[i + 1];
        
        for (int j = start_idx; j < end_idx; j++) 
        {
            int col_idx = csr->column_indexes[j];
            int element_val = csr->elements[j];
            if (i < std_mat->num_rows && col_idx < std_mat->num_cols) 
                std_mat->matrix[i][col_idx] = element_val;
        }
    }
    
    return SUCCESS;
}

int multiply_standard_matrices(const struct StandardMat *a, const struct StandardMat *b, struct StandardMat *result) 
{
    if (a->num_cols != b->num_rows) 
    {
        printf("Ошибка: число столбцов первой матрицы должно совпадать с числом строк второй матрицы\n");
        return MULT_ERROR;
    }
    
    result->num_rows = a->num_rows;
    result->num_cols = b->num_cols;
    
    result->matrix = malloc(result->num_rows * sizeof(int*));
    if (!result->matrix) 
        return INPUT_ERROR;
    
    for (int i = 0; i < result->num_rows; i++) 
    {
        result->matrix[i] = calloc(result->num_cols, sizeof(int));
        if (!result->matrix[i]) 
        {
            for (int j = 0; j < i; j++) 
                free(result->matrix[j]);
            free(result->matrix);
            return INPUT_ERROR;
        }
        
        for (int j = 0; j < result->num_cols; j++) 
        {
            for (int k = 0; k < a->num_cols; k++) 
            {
                result->matrix[i][j] += a->matrix[i][k] * b->matrix[k][j];
            }
        }
    }
    
    return SUCCESS;
}

int initialize_csc_format(struct CompactCSC *csc, int num_rows, int num_cols, int buffer_size) 
{
    csc->elements = malloc(buffer_size * sizeof(int));
    csc->row_indexes = malloc(buffer_size * sizeof(int));
    csc->col_starts = calloc(num_cols + 1, sizeof(int));
    
    if (!csc->elements || !csc->row_indexes || !csc->col_starts) 
    {
        free(csc->elements);
        free(csc->row_indexes);
        free(csc->col_starts);
        return INPUT_ERROR;
    }
    
    csc->element_count = 0;
    csc->buffer_size = buffer_size;
    csc->num_rows = num_rows;
    csc->num_cols = num_cols;
    
    return SUCCESS;
}

void release_csc_matrix(struct CompactCSC *csc) 
{
    if (csc->elements) 
    {
        free(csc->elements);
        csc->elements = NULL;
    }
    if (csc->row_indexes) 
    {
        free(csc->row_indexes);
        csc->row_indexes = NULL;
    }
    if (csc->col_starts) 
    {
        free(csc->col_starts);
        csc->col_starts = NULL;
    }
    csc->element_count = 0;
    csc->buffer_size = 0;
    csc->num_rows = 0;
    csc->num_cols = 0;
}

int convert_standard_to_csc(const struct StandardMat *std_mat, struct CompactCSC *csc) 
{
    int *column_counts = calloc(std_mat->num_cols, sizeof(int));
    if (!column_counts) return INPUT_ERROR;
    
    for (int j = 0; j < std_mat->num_cols; j++) 
    {
        for (int i = 0; i < std_mat->num_rows; i++) 
        {
            if (std_mat->matrix[i][j] != 0)
                column_counts[j]++;
        }
    }
    int total_non_zero = 0;
    for (int j = 0; j < std_mat->num_cols; j++) 
    {
        total_non_zero += column_counts[j];
    }
    
    if (initialize_csc_format(csc, std_mat->num_rows, std_mat->num_cols, total_non_zero) != SUCCESS) 
    {
        free(column_counts);
        return INPUT_ERROR;
    }
    csc->col_starts[0] = 0;
    for (int j = 1; j <= std_mat->num_cols; j++) 
    {
        csc->col_starts[j] = csc->col_starts[j - 1] + column_counts[j - 1];
    }
    
    int *current_position = calloc(std_mat->num_cols, sizeof(int));
    if (!current_position) 
    {
        free(column_counts);
        release_csc_matrix(csc);
        return INPUT_ERROR;
    }
    
    for (int j = 0; j < std_mat->num_cols; j++) 
    {
        for (int i = 0; i < std_mat->num_rows; i++) 
        {
            if (std_mat->matrix[i][j] != 0) 
            {
                int pos = csc->col_starts[j] + current_position[j];
                csc->elements[pos] = std_mat->matrix[i][j];
                csc->row_indexes[pos] = i;
                current_position[j]++;
            }
        }
    }
    
    csc->element_count = total_non_zero;
    
    free(column_counts);
    free(current_position);
    return SUCCESS;
}

void display_csc_format(const struct CompactCSC *csc) 
{
    printf("CSC представление матрицы %dx%d:\n", csc->num_rows, csc->num_cols);
    
    if (csc->element_count == 0) {
        printf("Матрица пуста\n");
        return;
    }
    
    printf("\nЭлементы (B): \n");
    for (int i = 0; i < csc->element_count; i++) 
    {
        printf("%d ", csc->elements[i]);
    }
    printf("\nНачала столбцов (IB): \n");
    for (int i = 0; i <= csc->num_cols; i++) 
    {
        printf("%d ", csc->col_starts[i]);
    }
    printf("\nИндексы строк (JB): \n");
    for (int i = 0; i < csc->element_count; i++) 
    {
        printf("%d ", csc->row_indexes[i]);
    }
    printf("\n");
}

int multiply_csr_csc_matrices(const struct CompactCSR *a, const struct CompactCSC *b, struct CompactCSR *res) 
{
    if (a->num_cols != b->num_rows) 
    {
        printf("Ошибка: число столбцов первой матрицы (%d) должно совпадать с числом строк второй матрицы (%d)\n", a->num_cols, b->num_rows);
        return MULT_ERROR;
    }
    int max_buffer = a->element_count * b->element_count;
    if (initialize_csr(res, a->num_rows, b->num_cols, max_buffer) != SUCCESS)
        return INPUT_ERROR;
    
    for (int i = 0; i < a->num_rows; i++) 
    {
        int a_start = a->row_starts[i];
        int a_end = a->row_starts[i + 1];
        
        for (int j = 0; j < b->num_cols; j++) 
        {
            int total = 0;
            int a_pos = a_start;
            int b_start = b->col_starts[j];
            int b_end = b->col_starts[j + 1];
            int b_pos = b_start;
            
            while (a_pos < a_end && b_pos < b_end) 
            {
                int a_col = a->column_indexes[a_pos];
                int b_row = b->row_indexes[b_pos];
                
                if (a_col < b_row) 
                {
                    a_pos++;
                } 
                else if (a_col > b_row) 
                {
                    b_pos++;
                } 
                else 
                {
                    total += a->elements[a_pos] * b->elements[b_pos];
                    a_pos++;
                    b_pos++;
                }
            }
            
            if (total != 0) 
            {
                if (insert_element_csr(res, i, j, total) != SUCCESS) 
                {
                    release_csr_matrix(res);
                    return INPUT_ERROR;
                }
            }
        }
    }
    
    return finalize_csr_structure(res);
}

size_t calculate_standard_memory(const struct StandardMat *mat) 
{
    size_t memory_used = 0;
    memory_used += mat->num_rows * sizeof(int*);
    memory_used += mat->num_rows * mat->num_cols * sizeof(int);
    memory_used += sizeof(struct StandardMat);
    return memory_used;
}

size_t calculate_csr_memory(const struct CompactCSR *mat) 
{
    size_t memory_used = 0;
    memory_used += mat->buffer_size * sizeof(int);
    memory_used += mat->buffer_size * sizeof(int);
    memory_used += (mat->num_rows + 1) * sizeof(int);
    memory_used += sizeof(struct CompactCSR);
    return memory_used;
}


void run_performance_test(void) 
{
    printf("Тестирование производительности и использования памяти\n");
    printf("Первая матрица - CSR, вторая матрица - CSC\n");
    
    int test_sizes[] = {50, 100, 200};
    double density_levels[] = {0.01, 0.1, 0.2, 0.3, 0.4, 0.5};
    int num_test_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);
    int num_density_levels = sizeof(density_levels) / sizeof(density_levels[0]);
    int test_iterations = 200;
    
    printf("Сравнение скорости умножения (время в наносекундах):\n");
    printf("Размер | Заполнение | Обычные(нс) | Разреженные(нс) | Эффективность(%%)\n");
    printf("-------+------------+-------------+-----------------+------------------\n");
    
    for (int s_idx = 0; s_idx < num_test_sizes; s_idx++) 
    {
        int current_size = test_sizes[s_idx];
        
        for (int f_idx = 0; f_idx < num_density_levels; f_idx++) 
        {
            double density = density_levels[f_idx];
            int non_zero_elements = (int)(current_size * current_size * density);
            if (non_zero_elements < 1) non_zero_elements = 1;
            
            struct StandardMat std_mat1 = {0}, std_mat2 = {0}, std_result = {0};
            struct CompactCSR csr_mat1 = {0}, csr_result = {0};
            struct CompactCSC csc_mat2 = {0};
            
            if (create_random_matrix(&std_mat1, current_size, current_size, non_zero_elements) != SUCCESS) 
                continue;
            
            if (create_random_matrix(&std_mat2, current_size, current_size, non_zero_elements) != SUCCESS) 
            {
                release_standard_matrix(&std_mat1);
                continue;
            }
            
            convert_standard_to_csr(&std_mat1, &csr_mat1);
            convert_standard_to_csc(&std_mat2, &csc_mat2);
            
            double total_std_time = 0;
            double total_sparse_time = 0;
            int valid_iterations = 0;
            
            for (int iter = 0; iter < test_iterations; iter++) 
            {
                clock_t start_time = clock();
                int result = multiply_standard_matrices(&std_mat1, &std_mat2, &std_result);
                clock_t end_time = clock();
                if (result == SUCCESS) 
                {
                    total_std_time += (double)(end_time - start_time) * (1000000000.0 / CLOCKS_PER_SEC);
                    release_standard_matrix(&std_result);
                    valid_iterations++;
                }
            }
            
            for (int iter = 0; iter < test_iterations; iter++) 
            {
                clock_t start_time = clock();
                int result = multiply_csr_csc_matrices(&csr_mat1, &csc_mat2, &csr_result);
                clock_t end_time = clock();
                if (result == SUCCESS) 
                {
                    total_sparse_time += (double)(end_time - start_time) * (1000000000.0 / CLOCKS_PER_SEC);
                    release_csr_matrix(&csr_result);
                }
            }
            
            if (valid_iterations > 0) 
            {
                double avg_std_time = total_std_time / valid_iterations;
                double avg_sparse_time = total_sparse_time / test_iterations;
                double performance_gain = 0;

                if (avg_std_time > 0) 
                    performance_gain = (avg_std_time - avg_sparse_time) / avg_std_time * 100;
                
                printf("%6d | %10.1f%% | %12.1f | %15.1f | %15.1f%%\n", 
                       current_size, density * 100, avg_std_time, avg_sparse_time, performance_gain);
                if (current_size == 100 && f_idx < num_density_levels - 1) 
                {
                    double current_gain = performance_gain;
                    if (fabs(current_gain) < 10.0) 
                    {
                        
                        double search_step = 0.01; 
                        double best_density = density;
                        double best_gain = current_gain;
                        int search_direction = (current_gain > 0) ? 1 : -1;
                        
                        for (int refinement = 0; refinement < 3; refinement++) 
                        {
                            double test_density = best_density + search_direction * search_step;
                            
                            if (test_density <= 0 || test_density >= 1.0) 
                                break;
                                
                            int test_non_zero = (int)(current_size * current_size * test_density);
                            if (test_non_zero < 1) test_non_zero = 1;
                            
                            struct StandardMat test_std1 = {0}, test_std2 = {0}, test_std_res = {0};
                            struct CompactCSR test_csr1 = {0}, test_csr_res = {0};
                            struct CompactCSC test_csc2 = {0};
                            
                            if (create_random_matrix(&test_std1, current_size, current_size, test_non_zero) == SUCCESS &&
                                create_random_matrix(&test_std2, current_size, current_size, test_non_zero) == SUCCESS) 
                            {
                                convert_standard_to_csr(&test_std1, &test_csr1);
                                convert_standard_to_csc(&test_std2, &test_csc2);
                                
                                double test_std_time = 0;
                                double test_sparse_time = 0;
                                int test_count = 0;
                                
                                for (int iter = 0; iter < 20; iter++) 
                                {
                                    clock_t start = clock();
                                    if (multiply_standard_matrices(&test_std1, &test_std2, &test_std_res) == SUCCESS) 
                                    {
                                        clock_t end = clock();
                                        test_std_time += (double)(end - start) * (1000000000.0 / CLOCKS_PER_SEC);
                                        release_standard_matrix(&test_std_res);
                                        test_count++;
                                    }
                                    
                                    start = clock();
                                    if (multiply_csr_csc_matrices(&test_csr1, &test_csc2, &test_csr_res) == SUCCESS) 
                                    {
                                        clock_t end = clock();
                                        test_sparse_time += (double)(end - start) * (1000000000.0 / CLOCKS_PER_SEC);
                                        release_csr_matrix(&test_csr_res);
                                    }
                                }
                                
                                if (test_count > 0) 
                                {
                                    double avg_test_std = test_std_time / test_count;
                                    double avg_test_sparse = test_sparse_time / 20;
                                    double test_gain = 0;
                                    
                                    if (avg_test_std > 0) 
                                        test_gain = (avg_test_std - avg_test_sparse) / avg_test_std * 100;
                                    
                                    printf("%6d | %10.1f%% | %12.1f | %15.1f | %15.1f%%\n", 
                                           current_size, test_density * 100, avg_test_std, avg_test_sparse, test_gain);
                                    if (fabs(test_gain) < fabs(best_gain)) 
                                    {
                                        best_gain = test_gain;
                                        best_density = test_density;
                                    }
                                    else 
                                    {
                                        search_direction *= -1;
                                        search_step *= 0.5;
                                    }
                                }
                                
                                release_standard_matrix(&test_std1);
                                release_standard_matrix(&test_std2);
                                release_csr_matrix(&test_csr1);
                                release_csc_matrix(&test_csc2);
                            }
                            if (fabs(best_gain) < 2.0 || search_step < 0.001) 
                                break;
                        }
                    }
                }
            }
            
            release_standard_matrix(&std_mat1);
            release_standard_matrix(&std_mat2);
            release_csr_matrix(&csr_mat1);
            release_csc_matrix(&csc_mat2);
        }
    }
    printf("\nСравнение использования памяти (в байтах):\n");
    printf("Размер | Заполнение | Обычные  | Разреженные | Эффективность(%%)\n");
    printf("-------+------------+----------+-------------+------------------\n");
    
    for (int s_idx = 0; s_idx < num_test_sizes; s_idx++) 
    {
        int current_size = test_sizes[s_idx];
        
        for (int f_idx = 0; f_idx < num_density_levels; f_idx++) 
        {
            double density = density_levels[f_idx];
            int non_zero_elements = (int)(current_size * current_size * density);
            if (non_zero_elements < 1) non_zero_elements = 1;
            
            struct StandardMat std_mat = {0};
            struct CompactCSR csr_mat = {0};
            
            if (create_random_matrix(&std_mat, current_size, current_size, non_zero_elements) != SUCCESS) 
                continue;
            
            if (convert_standard_to_csr(&std_mat, &csr_mat) != SUCCESS) 
            {
                release_standard_matrix(&std_mat);
                continue;
            }

            long long std_memory = (long long)calculate_standard_memory(&std_mat);
            long long sparse_memory = (long long)calculate_csr_memory(&csr_mat);
            double memory_saving = 0;

            if (std_memory > 0)
                memory_saving = (double)(std_memory - sparse_memory) / std_memory * 100;
            
            printf("%6d | %10.1f%% | %9lld | %12lld | %15.1f%%\n", 
                   current_size, density * 100, std_memory, sparse_memory, memory_saving);
            
            release_standard_matrix(&std_mat);
            release_csr_matrix(&csr_mat);
        }
    }
}
void show_menu(void) 
{
    struct StandardMat std_mat1 = {0}, std_mat2 = {0}, std_result = {0};
    struct CompactCSR csr_mat1 = {0}, csr_result = {0};
    struct CompactCSC csc_mat2 = {0};
    int matrices_ready = 0;
    int user_choice;

    MENU_LOOP:

    printf("\n");
    printf("Главное меню\n");
    printf("1 - Ввод матриц вручную\n");
    printf("2 - Генерация случайных матриц\n");
    printf("3 - Умножение матриц (стандартный формат)\n");
    printf("4 - Умножение матриц (CSR * CSC формат)\n");
    printf("5 - Отображение матриц (стандартный формат)\n");
    printf("6 - Отображение матриц (разреженный формат)\n");
    printf("7 - Запуск теста производительности\n");
    printf("0 - Завершение работы\n");
    printf("Выберите действие: ");
    
    if (scanf("%d", &user_choice) != 1) 
    {
        printf("Ошибка ввода\n");
        clean_input_buffer();
        goto MENU_LOOP;
    }

    clean_input_buffer();

    if (user_choice == 1) 
    {
        release_standard_matrix(&std_mat1);
        release_standard_matrix(&std_mat2);
        release_csr_matrix(&csr_mat1);
        release_csc_matrix(&csc_mat2);
        
        printf("Ввод первой матрицы (будет сохранена в CSR):\n");
        if (read_standard_matrix(&std_mat1, -1) != SUCCESS) 
        {
            printf("Ошибка ввода матрицы 1!\n");
            goto MENU_LOOP;
        }
        convert_standard_to_csr(&std_mat1, &csr_mat1);
        clean_input_buffer();
        
        printf("Ввод второй матрицы (будет сохранена в CSC):\n");
        if (read_standard_matrix(&std_mat2, std_mat1.num_cols) != SUCCESS) 
        {
            printf("Ошибка ввода матрицы 2!\n");
            release_standard_matrix(&std_mat1);
            release_csr_matrix(&csr_mat1);
            goto MENU_LOOP;
        }
        convert_standard_to_csc(&std_mat2, &csc_mat2);
        
        matrices_ready = 1;
        printf("Матрицы успешно загружены\n");
        printf("Первая матрица: %dx%d (CSR формат)\n", std_mat1.num_rows, std_mat1.num_cols);
        printf("Вторая матрица: %dx%d (CSC формат)\n", std_mat2.num_rows, std_mat2.num_cols);
        goto MENU_LOOP;
    }
    else if (user_choice == 2) 
    {
        release_standard_matrix(&std_mat1);
        release_standard_matrix(&std_mat2);
        release_csr_matrix(&csr_mat1);
        release_csc_matrix(&csc_mat2);
        
        int rows1, cols1, non_zero1;
        int cols2, non_zero2;
        
        printf("Генерация первой матрицы (CSR):\n");
        printf("Введите число строк: ");
        if (scanf("%d", &rows1) != 1 || rows1 <= 0 || rows1 > MAX_DIMENSION) 
        {
            printf("Ошибка: некорректное число строк\n");
            clean_input_buffer();
            goto MENU_LOOP;
        }
        
        printf("Введите число столбцов: ");
        if (scanf("%d", &cols1) != 1 || cols1 <= 0 || cols1 > MAX_DIMENSION) 
        {
            printf("Ошибка: некорректное число столбцов\n");
            clean_input_buffer();
            goto MENU_LOOP;
        }
        
        printf("Введите число ненулевых элементов: ");
        if (scanf("%d", &non_zero1) != 1 || non_zero1 < 1) 
        {
            printf("Ошибка: некорректное число ненулевых элементов\n");
            clean_input_buffer();
            goto MENU_LOOP;
        }

        printf("Генерация второй матрицы (CSC):\n");
        printf("Число строк второй матрицы автоматически установлено: %d\n", cols1);
        printf("Введите число столбцов: ");
        if (scanf("%d", &cols2) != 1 || cols2 <= 0 || cols2 > MAX_DIMENSION) 
        {
            printf("Ошибка: некорректное число столбцов\n");
            clean_input_buffer();
            goto MENU_LOOP;
        }
        
        printf("Введите число ненулевых элементов: ");
        if (scanf("%d", &non_zero2) != 1 || non_zero2 < 1) 
        {
            printf("Ошибка: некорректное число ненулевых элементов\n");
            clean_input_buffer();
            goto MENU_LOOP;
        }
        
        if (create_random_matrix(&std_mat1, rows1, cols1, non_zero1) != SUCCESS) 
        {
            printf("Ошибка генерации первой матрицы!\n");
            goto MENU_LOOP;
        }
        convert_standard_to_csr(&std_mat1, &csr_mat1);
        
        if (create_random_matrix(&std_mat2, cols1, cols2, non_zero2) != SUCCESS) 
        {
            printf("Ошибка генерации второй матрицы!\n");
            release_standard_matrix(&std_mat1);
            release_csr_matrix(&csr_mat1);
            goto MENU_LOOP;
        }
        convert_standard_to_csc(&std_mat2, &csc_mat2);
        
        matrices_ready = 1;
        printf("Случайные матрицы успешно сгенерированы\n");
        printf("Первая матрица: %dx%d (CSR формат)\n", std_mat1.num_rows, std_mat1.num_cols);
        printf("Вторая матрица: %dx%d (CSC формат)\n", std_mat2.num_rows, std_mat2.num_cols);
        goto MENU_LOOP;
    }
    else if (user_choice == 3) 
    {
        if (!matrices_ready) 
        {
            printf("Ошибка: матрицы не загружены!\n");
            goto MENU_LOOP;
        }
        
        release_standard_matrix(&std_result);
        if (multiply_standard_matrices(&std_mat1, &std_mat2, &std_result) == SUCCESS) 
        {
            printf("Результат умножения матриц (стандартный формат):\n");
            display_standard_matrix(&std_result);
        } 
        else 
        {
            if (std_mat1.num_cols != std_mat2.num_rows) 
            {
                printf("Невозможно умножить: число столбцов первой матрицы (%d) не равно числу строк второй матрицы (%d)\n", 
                       std_mat1.num_cols, std_mat2.num_rows);
            }
        }
        goto MENU_LOOP;
    }
    else if (user_choice == 4) 
    {
        if (!matrices_ready) 
        {
            printf("Ошибка: матрицы не загружены!\n");
            goto MENU_LOOP;
        }
        
        release_csr_matrix(&csr_result);
        if (multiply_csr_csc_matrices(&csr_mat1, &csc_mat2, &csr_result) == SUCCESS) 
        {
            printf("Результат умножения разреженных матриц (CSR * CSC):\n");
            display_csr_format(&csr_result);
            
            release_standard_matrix(&std_result);
            convert_csr_to_standard(&csr_result, &std_result);
            printf("\nТот же результат в стандартном формате:\n");
            display_standard_matrix(&std_result);
        } 
        else 
        {
            printf("Ошибка умножения разреженных матриц!\n");
            if (csr_mat1.num_cols != csc_mat2.num_rows) 
            {
                printf("Невозможно умножить: число столбцов первой матрицы (%d) не равно числу строк второй матрицы (%d)\n", 
                       csr_mat1.num_cols, csc_mat2.num_rows);
            }
        }
        goto MENU_LOOP;
    }
    else if (user_choice == 5) 
    {
        if (!matrices_ready) 
        {
            printf("Ошибка: матрицы не загружены!\n");
            goto MENU_LOOP;
        }
        
        printf("Первая матрица:\n");
        display_standard_matrix(&std_mat1);
        printf("Вторая матрица:\n");
        display_standard_matrix(&std_mat2);
        if (std_result.num_rows > 0) 
        {
            printf("Результат умножения:\n");
            display_standard_matrix(&std_result);
        }
        goto MENU_LOOP;
    }
    else if (user_choice == 6) 
    {
        if (!matrices_ready) 
        {
            printf("Ошибка: матрицы не загружены!\n");
            goto MENU_LOOP;
        }
        
        printf("Первая матрица в формате CSR:\n");
        display_csr_format(&csr_mat1);
        printf("\nВторая матрица в формате CSC:\n");
        display_csc_format(&csc_mat2);
        if (csr_result.num_rows > 0) 
        {
            printf("\nРезультат умножения в формате CSR:\n");
            display_csr_format(&csr_result);
        }
        goto MENU_LOOP;
    }
    else if (user_choice == 7) 
    {
        run_performance_test();
        goto MENU_LOOP;
    }
    else if (user_choice == 0) 
    {
        printf("Завершение работы программы\n");
    }
    else 
    {
        printf("Неверный выбор! Попробуйте снова.\n");
        goto MENU_LOOP;
    }

    release_standard_matrix(&std_mat1);
    release_standard_matrix(&std_mat2);
    release_standard_matrix(&std_result);
    release_csr_matrix(&csr_mat1);
    release_csc_matrix(&csc_mat2);
    release_csr_matrix(&csr_result);
}
