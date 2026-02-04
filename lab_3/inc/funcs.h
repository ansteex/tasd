#ifndef FUNC
#define FUNC

#include "consts.h"

int read_coordinate_matrix(struct StandardMat *mat);
int read_standard_matrix(struct StandardMat *mat, int fixed_rows);
void display_standard_matrix(const struct StandardMat *mat);
void display_csr_format(const struct CompactCSR *csr);
void release_standard_matrix(struct StandardMat *mat);
void release_csr_matrix(struct CompactCSR *csr);
int initialize_csr(struct CompactCSR *csr, int rows, int cols, int capacity);
int insert_element_csr(struct CompactCSR *csr, int row, int col, int val);

int convert_standard_to_csr(const struct StandardMat *std_mat, struct CompactCSR *csr);
int convert_csr_to_standard(const struct CompactCSR *csr, struct StandardMat *std_mat);

int multiply_standard_matrices(const struct StandardMat *a, const struct StandardMat *b, struct StandardMat *result);
int initialize_csc_format(struct CompactCSC *csc, int rows, int cols, int capacity);
void release_csc_matrix(struct CompactCSC *csc);
int convert_standard_to_csc(const struct StandardMat *std_mat, struct CompactCSC *csc);
void display_csc_format(const struct CompactCSC *csc);
int multiply_csr_csc_matrices(const struct CompactCSR *a, const struct CompactCSC *b, struct CompactCSR *res);
void clean_input_buffer(void);
int create_random_matrix(struct StandardMat *mat, int rows, int cols, int non_zero_count);
size_t calculate_csc_memory(const struct CompactCSC *mat);
void run_performance_test(void);
size_t calculate_standard_memory(const struct StandardMat *mat);
size_t calculate_csr_memory(const struct CompactCSR *mat);

void show_menu(void);

#endif
