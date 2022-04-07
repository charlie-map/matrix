#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "matrix.h"

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

		for (int load_val = 0; load_val < col; load_val++) {
			matrix[set_col][load_val] = 0;
		}
	}

	matrix_t *r_m = malloc(sizeof(matrix_t));

	r_m->row = row;
	r_m->col = col;

	r_m->matrix = matrix;

	return r_m;
}

int matrix_load(matrix_t *m, float **input) {
	for (int i = 0; i < m->row; i++) {
		for (int j = 0; j < m->col; j++) {
			m->matrix[i][j] = input[i][j];
		}
	}

	return 0;
}

// copies values in matrix m and returns the copy
matrix_t *matrix_copy(matrix_t *m) {
	matrix_t *n_m = matrix_build(m->row, m->col);

	// copy values into n_m
	for (int i = 0; i < m->row; i++) {
		for (int j = 0; j < m->col; j++) {

			n_m->matrix[i][j] = m->matrix[i][j];
		}
	}

	return n_m;
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
			if (r_m)
				r_m->matrix[add_row][add_col] = is_add ? m1->matrix[add_row][add_col] + m2->matrix[add_row][add_col] :
					m1->matrix[add_row][add_col] - m2->matrix[add_row][add_col];
			else
				m1->matrix[add_row][add_col] = is_add ? m1->matrix[add_row][add_col] + m2->matrix[add_row][add_col] :
					m1->matrix[add_row][add_col] - m2->matrix[add_row][add_col];
		}
	}

	return 0;
}

// adder_p: defines if results should occur in m1, or if results
// should be placed in a new matrix. defaults to subtracting
// and placing results in m1. Add "-n" to instead place results
// into new matrix
matrix_t *matrix_add(matrix_t *m1, matrix_t *m2, char *adder_p) {
	if (!check_dimensions_add_sub(m1, m2))
		return NULL;

	int make_new = adder_p[0] == '-' && adder_p[1] == 'n';

	matrix_t *r_m = NULL;
	if (make_new)
		r_m = matrix_build(m1->row, m1->col);

	matrix_add_sub_helper(r_m, m1, m2, 1);

	return r_m;
}

matrix_t *matrix_subtract(matrix_t *m1, matrix_t *m2, char *subber_p) {
	if (!check_dimensions_add_sub(m1, m2))
		return NULL;

	int make_new = subber_p[0] == '-' && subber_p[1] == 'n';

	matrix_t *r_m = NULL;
	if (make_new)
		r_m = matrix_build(m1->row, m1->col);

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

int matrix_multiply_scaler(matrix_t *m1, float scaler) {
	for (int i = 0; i < m1->row; i++) {
		for (int j = 0; j < m1->col; j++) {
			m1->matrix[i][j] *= scaler;
		}
	}

	return 0;
}

int matrix_print(matrix_t *m) {
	int col_num = m->col;

	printf("|");
	for (int add_dash = 0; add_dash < col_num; add_dash++)
		printf("-------%c", add_dash != col_num - 1 ? '-' : '|');
	printf("\n");


	int title_len = (col_num * 8) - 16;
	printf("|");
	for (int add_title = 0; add_title < title_len * 0.5; add_title++)
		printf(" ");
	printf("(%d)x(%d) Matrix", m->row, m->col);
	for (int add_title = 0; add_title < title_len * 0.5; add_title++)
		printf(" ");
	printf(" |\n");

	printf("|");
	for (int add_dash = 0; add_dash < col_num; add_dash++)
		printf("-------%c", add_dash != col_num - 1 ? '-' : '|');
	printf("\n");

	for (int print_row = 0; print_row < m->row; print_row++) {

		printf("|");
		for (int print_col = 0; print_col < m->col; print_col++) {
			printf(" %01.3f |", m->matrix[print_row][print_col]);
		}
		printf("\n");
	}

	printf("|");
	for (int add_dash = 0; add_dash < col_num; add_dash++)
		printf("-------%c", add_dash != col_num - 1 ? '-' : '|');
	printf("\n");

	return 0;
}


int partial_derivative(int cols, matrix_t *m1, matrix_t *expected, matrix_t *variable, matrix_t *d_variable) {

	// compute m1Xvariable
	matrix_t *m1_variable = matrix_multiply(m1, variable);
	matrix_print(m1);
	matrix_print(variable);
	matrix_print(m1_variable);
	// first comput the sum of:
	// (estimated - expected) * m1_j
	// for all values i = 0 -> cols
	// for each weight value v in d_variable
	float sum;
	for (int v = 0; v < d_variable->row; v++) {
		sum = 0;

		for (int i = 0; i < cols; i++) {
			// compute error of estimated versus expected
			float error = m1_variable->matrix[i][0] - expected->matrix[i][0];
			error *= m1->matrix[v][i];

			sum += error;
		}

		sum /= cols;

		d_variable->matrix[v][0] = sum;
	}

	return 0;
}
/*
	Matrix Gradient Descent:

		Takes in m1, which is a fully formed mxn matrix,
	variable, which is an empty 1xn matrix, and expected, which is
	the desired 1xn output. The algorithm continuously trys to move the
	values of variable to get the output of m1Xvariables as close
	to expected as possible

	At the end, variable will have the results within it.
*/
int matrix_gradient_descent(matrix_t *m1, matrix_t *variable, matrix_t *expected) {
	for (int set = 0; set < variable->row; set++)
		variable->matrix[set][0] = 0;

	int col = m1->col;

	if (col != variable->row)
		return 0;

	// d_variable for finding partial derivative:
	matrix_t *d_variable = matrix_build(variable->row, 1);
	int iteration_max = 30, iter = 0;
	while (iter < iteration_max) {
		partial_derivative(col, m1, expected, variable, d_variable);
		// subtract d_variable from variable:
		matrix_multiply_scaler(d_variable, 0.5);
		matrix_subtract(variable, d_variable, "");

		iter++;
	}

	return 0;
}