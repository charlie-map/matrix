#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

struct Matrix {
	// length of rows and cols
	int row, col;

	float **matrix;
};

/*
	builds and returns a simple matrix:

	row and col define the dimensions of the given matrix
*/
matrix_t *matrix_build(int row, int col) {
	float **matrix = malloc(sizeof(float *) * row);

	for (int set_col = 0; set_col < row; set_col++) {
		matrix[set_col] = malloc(sizeof(float) * col);
	}

	matrix_t *r_m = malloc(sizeof(matrix_t));

	r_m->row = row;
	r_m->col = col;

	r_m->matrix = matrix;

	return r_m;
}

int check_dimensions_add_sub(matrix_t *m1, matrix_t *m2) {
	// check dimensions
	if (m1->row != m2->row)
		return 0;

	if (m1->col != m2->col)
		return 0;

	return 1;
}

int matrix_add_sub_helper(matrix_t *r_m, matrix_t *m1, matrix_t *m2, int is_add) {
	for (int add_row = 0; add_row < m1->row; add_row++) {

		for (int add_col = 0; add_col < m1->col; add_col++) {
			r_m->matrix[add_row][add_col] = is_add ? m1->matrix[add_row][add_col] + m1->matrix[add_row][add_col] :
			m1->matrix[add_row][add_col] - m1->matrix[add_row][add_col];
		}
	}

	return 0;
}

matrix_t *matrix_add(matrix_t *m1, matrix_t *m2) {
	if (!check_dimensions_add_sub(m1, m2))
		return NULL;

	matrix_t *r_m = matrix_build(m1->row, m1->col);

	matrix_add_sub_helper(r_m, m1, m2, 1);

	return r_m;
}

matrix_t *matrix_subtract(matrix_t *m1, matrix_t *m2) {
	if (!check_dimensions_add_sub(m1, m2))
		return NULL;

	matrix_t *r_m = matrix_build(m1->row, m1->col);

	matrix_add_sub_helper(r_m, m1, m2, 0);

	return r_m;
}

int check_dimensions_multi(matrix_t *m1, matrix_t *m2) {
	if (m1->col != m2->row)
		return 0;

	return 1;
}

matrix_t *matrix_multiply(matrix_t *m1, matrix_t *m2) {
	if (!check_dimensions_multi(m1, m2))
		return NULL;

	matrix_t *r_m = matrix_build(m1->row, m2->col);
	for (int calc_row = 0; calc_row < m1->row; calc_row++) {
		for (int calc_col = 0; calc_col < m2->col; calc_col++) {

			r_m->matrix[calc_row][calc_col] = 0;
			for (int multi = 0; multi < m1->col; multi++) {
				r_m->matrix[calc_row][calc_col] += m1->matrix[calc_row][multi] * m2->matrix[multi][calc_col];
			}
		}
	}

	return r_m;
}

matrix_t *matrix_divide(matrix_t *m1, matrix_t *m2) {

}