#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

struct Matrix {
	int *length, dimension_num;
	void *matrix;
};

void set_sub_matrix(void *matrix, int *len, int curr_dim, int max_dim) {
	if (curr_dim >= max_dim)
		return;

	for (int setter = 0; setter < len[curr_dim - 1]; setter++) {
		matrix[setter] = malloc(sizeof(void *) * len[curr_dim]);
		set_sub_matrix(matrix[setter], len, curr_dim + 1, max_dim);
	}
}

matrix_t *matrix_build(int dimension_num, ...) {
	va_list build_arg;
	va_start(build_arg, dimension_num);

	int *len = malloc(sizeof(int) * dimension_num);
	for (int set_matrix = 0; set_matrix < dimension_num; set_matrix++)
		len[set_matrix] = va_arg(build_arg, int);

	void *matrix = malloc(sizeof(void *) * len[0]);
	set_sub_matrix(matrix, len, 1, dimension_num);

	matrix_t *m = malloc(sizeof(matrix_t));

	m->dimension_num = dimension_num;
	m->length = len;

	m->matrix = matrix;

	return m;
}

int matrix_update(matrix_t *m, ...) {
	va_list update_arg;
	va_start(update_arg, m);

	void *matrix_mover = m->matrix;
	int dimension_pos;
	for (int dimension = 0; dimension < dimension_num; dimension++) {

		dimension_pos = va_arg(update_arg, int);

		matrix_mover = ((void **) matrix_mover)[dimension_pos];
	}

	m->matrix_set(matrix_mover, va_arg(update_arg, void *));

	return 0;
}

int matrix_add_helper(void *r_m, void *m1, void *m2, int *len, int curr_dim, int max_dim) {
	if (curr_dim >= max_dim)
		return 0;

	// level of computation
	if (curr_dim == max_dim - 1) {
		for (int add = 0; add < len[curr_dim]; add++) {
			r_m[add] = m1[add] + m2[add];
		}

		return 0;
	}

	for (int sub_add = 0; sub_add < len[curr_dim] sub_add++) {
		matrix_add_helper(r_m[sub_add], m1[sub_add], m2[sub_add], len, curr_dim + 1, max_dim);
	}

	return 0;
}

matrix_t *matrix_add(matrix_t *m1, matrix_t *m2) {
	// check dimensions
	if (m1->dimension_num != m2->dimension_num)
		return NULL;

	for (int check_dim_len = 0; check_dim_len < m1->dimension_num; check_dim_len++) {
		if (m1->length[check_dim_len] != m2->length[check_dim_len])
			return NULL;
	}

	// build return matrix
	matrix_t *r_m = matrix_build(m1->dimension_num, m1->length);

	matrix_add_helper(r_m, m1, m2, m1->length, 0, m1->dimension_num);

	return r_m;
}

matrix_t *matrix_subtract(matrix_t *m1, matrix_t *m2) {
	
}

matrix_t *matrix_multiply(matrix_t *m1, matrix_t *m2) {

}

matrix_t *matrix_divide(matrix_t *m1, matrix_t *m2) {

}