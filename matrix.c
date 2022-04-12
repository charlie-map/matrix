#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "matrix.h"

struct Matrix {
	// length of x and y
	int width, height;

	float **matrix;
};

/*
	builds and returns a simple matrix:

	width and height define the dimensions of the given matrix
*/
matrix_t *matrix_build(int width, int height) {
	float **matrix = malloc(sizeof(float *) * width);

	for (int x = 0; x < width; x++) {
		matrix[x] = malloc(sizeof(float) * height);

		for (int y = 0; y < height; y++) {
			matrix[x][y] = 0;
		}
	}

	matrix_t *r_m = malloc(sizeof(matrix_t));

	r_m->width = width;
	r_m->height = height;

	r_m->matrix = matrix;

	return r_m;
}

// creats identity matrix of size nxn
matrix_t *matrix_identity(int n) {
	matrix_t *m_i = malloc(sizeof(matrix_t));

	m_i->width = n;
	m_i->height = n;
	float **m_i_v = malloc(sizeof(float *) * n);

	for (int x = 0; x < n; x++) {
		m_i_v[x] = malloc(sizeof(float) * n);

		for (int y = 0; y < n; y++) {

			m_i_v[x][y] = x == y;
		}
	}

	m_i->matrix = m_i_v;

	return m_i;
}

int matrix_load(matrix_t *m, float **input) {
	for (int x = 0; x < m->width; x++) {
		for (int y = 0; y < m->height; y++) {
			m->matrix[x][y] = input[x][y];
		}
	}

	return 0;
}

// copies values in matrix m and returns the copy
matrix_t *matrix_copy(matrix_t *m) {
	matrix_t *n_m = matrix_build(m->width, m->height);

	// copy values into n_m
	for (int x = 0; x < m->width; x++) {
		for (int y = 0; y < m->height; y++) {

			n_m->matrix[x][y] = m->matrix[x][y];
		}
	}

	return n_m;
}

float *matrix_get_row(matrix_t *A, int j) {
	float *row = malloc(sizeof(float) * A->width);

	for (int x = 0; x < A->width; x++)
		row[x] = A->matrix[x][j];

	return row;
}

float *matrix_get_col(matrix_t *A, int i) {
	float *col = malloc(sizeof(float) * A->height);

	for (int y = 0; y < A->height; y++)
		col[y] = A->matrix[i][y];

	return col;
}

int check_dimensions_add_sub(matrix_t *m1, matrix_t *m2) {
	// check dimensions
	if (m1->width != m2->width)
		return 0;

	if (m1->height != m2->height)
		return 0;

	return 1;
}

int matrix_add_sub_helper(matrix_t *r_m, matrix_t *m1, matrix_t *m2, int is_add) {
	for (int x = 0; x < m1->width; x++) {

		for (int y = 0; y < m1->height; y++) {
			if (r_m)
				r_m->matrix[x][y] = is_add ? m1->matrix[x][y] + m2->matrix[x][y] :
					m1->matrix[x][y] - m2->matrix[x][y];
			else
				m1->matrix[x][y] = is_add ? m1->matrix[x][y] + m2->matrix[x][y] :
					m1->matrix[x][y] - m2->matrix[x][y];
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
		r_m = matrix_build(m1->width, m1->height);

	matrix_add_sub_helper(r_m, m1, m2, 1);

	return r_m;
}

/*
	takes in specific row j and adds a given float *, s, from that row
	in the inputted matrix, A
*/
int matrix_add_row(matrix_t *A, float *s, int j) {
	for (int x = 0; x < A->width; x++)
		A->matrix[x][j] += s[x];

	return 0;
}

matrix_t *matrix_subtract(matrix_t *m1, matrix_t *m2, char *subber_p) {
	if (!check_dimensions_add_sub(m1, m2))
		return NULL;

	int make_new = subber_p[0] == '-' && subber_p[1] == 'n';

	matrix_t *r_m = NULL;
	if (make_new)
		r_m = matrix_build(m1->width, m1->height);

	matrix_add_sub_helper(r_m, m1, m2, 0);

	return r_m;
}

/*
	takes in specific row j and subtracts a given float *, s, from that row
	in the inputted matrix, A
*/
int matrix_subtract_row(matrix_t *A, float *s, int j) {
	for (int x = 0; x < A->width; x++)
		A->matrix[x][j] -= s[x];

	return 0;
}

matrix_t *matrix_transpose(matrix_t *m) {
	matrix_t *m_t = matrix_build(m->height, m->width);
	float **m_t_value = malloc(sizeof(float *) * m->height);

	for (int y = 0; y < m->height; y++) {
		m_t_value[y] = malloc(sizeof(float) * m->width);

		for (int x = 0; x < m->width; x++) {
			m_t_value[y][x] = m->matrix[x][y];
		}
	}

	matrix_load(m_t, m_t_value);

	return m_t;
}

int check_dimensions_multi(matrix_t *m1, matrix_t *m2) {
	if (m1->width != m2->height)
		return 0;

	return 1;
}

// comments made with the example 3x3 m1, 1x3 m2
matrix_t *matrix_multiply(matrix_t *m1, matrix_t *m2) {
	if (!check_dimensions_multi(m1, m2))
		return NULL;

	// create return matrix as 3x1 matrix
	matrix_t *r_m = matrix_build(m2->width, m1->height);

	// loop through each position in r_m and
	// compute value
	for (int r_m_x = 0; r_m_x < r_m->width; r_m_x++) {
		for (int r_m_y = 0; r_m_y < r_m->height; r_m_y++) {

			r_m->matrix[r_m_x][r_m_y] = 0;
			for (int multi = 0; multi < m1->width; multi++) {
				r_m->matrix[r_m_x][r_m_y] += m1->matrix[multi][r_m_y] * m2->matrix[r_m_x][multi];
			}
		}
	}

	return r_m;
}

/*
	takes in matrix A and multiplies A[x][y] by s

	if i and j are -1 that means do it for that entire row/column

	if i and j are set values that means only do the scaling
	for that specific ith column or jth row
*/
int matrix_multiply_scaler_helper(matrix_t *A, float s, int i, int j) {
	for (int x = i == -1 ? 0 : i; x < i == -1 ? A->width : i + 1; x++) {

		for (int y = j == -1 ? 0 : j; y < j == -1 ? A->height : j + 1; y++) {
			A->matrix[x][y] *= s;
		}
	}

	return 0;
}

int matrix_multiply_scaler(matrix_t *m1, float scaler) {
	matrix_multiply_scaler_helper(m1, scaler, -1, -1);

	return 0;
}

int matrix_multiply_scaler_row(matrix_t *A, float s, int j) {
	matrix_multiply_scaler_helper(A, s, -1, j);

	return 0;
}

int matrix_multiply_scaler_col(matrix_t *A, float s, int i) {
	matrix_multiply_scaler_helper(A, s, i, -1);

	return 0;
}

matrix_t *matrix_merge_col(matrix_t *m1, matrix_t *m2) {
	// check matrix height
	if (m1->height != m2->height)
		return NULL;

	// create new matrix:
	matrix_t *merge_m = matrix_build(m1->width + m2->width, m1->height);

	float **merge_m_v = malloc(sizeof(float *) * (m1->width + m2->width));
	for (int merge_m1_x = 0; merge_m1_x < m1->width + m2->width; merge_m1_x++) {

		merge_m_v[merge_m1_x] = malloc(sizeof(float) * m1->height);
		for (int merge_m1_y = 0; merge_m1_x < m1->width && merge_m1_y < m1->height; merge_m1_y++) {
			merge_m_v[merge_m1_x][merge_m1_y] = m1->matrix[merge_m1_x][merge_m1_y];
		}

		for (int merge_m2_y = 0; merge_m1_x >= m1->width && merge_m2_y < m2->height; merge_m2_y++) {
			merge_m_v[merge_m1_x][merge_m2_y] = m2->matrix[merge_m1_x - m1->width][merge_m2_y];
		}
	}

	matrix_load(merge_m, merge_m_v);
	return merge_m;
}

matrix_t *matrix_merge_row(matrix_t *m1, matrix_t *m2) {
	// check matrix width
	if (m1->width != m2->width)
		return NULL;

	// create new matrix:
	matrix_t *merge_m = matrix_build(m1->width, m1->height + m2->height);

	float **merge_m_v = malloc(sizeof(float *) * (m1->width));
	for (int merge_x = 0; merge_x < m1->width; merge_x++) {

		merge_m_v[merge_x] = malloc(sizeof(float) * (m1->height + m2->height));
		for (int merge_y = 0; merge_y < m1->height + m2->height; merge_y++) {
			float add_v;

			if (merge_y < m1->height)
				add_v = m1->matrix[merge_x][merge_y];
			else
				add_v = m2->matrix[merge_x][merge_y - m1->height];

			merge_m_v[merge_x][merge_y] = add_v;
		}
	}

	matrix_load(merge_m, merge_m_v);
	return merge_m;
}
/*
	takes in two matrices and splices the second matrix onto the
	end of m1, either as additional columns or as additional rows.
	m_dir is 'c' for adding columns and 'r' for adding rows
*/
matrix_t *matrix_merge(matrix_t *m1, matrix_t *m2, char m_dir) {
	if (m_dir == 'c')
		return matrix_merge_col(m1, m2);
	else if (m_dir == 'r')
		return matrix_merge_row(m1, m2);
	else
		return NULL;
}

matrix_t *matrix_splice_col(matrix_t *A, int start, int end) {
	matrix_t *sub_A = matrix_build(end - start, A->height);

	float **sub_A_val = malloc(sizeof(float *) * (end - start));

	for (int x = start; x < end; x++) {

		sub_A_val[x - start] = malloc(sizeof(float) * A->height);

		for (int y = 0; y < A->height; y++) {
			sub_A_val[x - start][y] = A->matrix[x][y];
		}
	}

	matrix_load(sub_A, sub_A_val);
	return sub_A;
}

matrix_t *matrix_splice_row(matrix_t *A, int start, int end) {
	matrix_t *sub_A = matrix_build(A->width, end - start);

	float **sub_A_val = malloc(sizeof(float *) * A->width);

	for (int x = 0; x < A->width; x++) {

		sub_A_val[x] = malloc(sizeof(float) * (end - start));

		for (int y = start; y < end; y++) {
			sub_A_val[x][y - start] = A->matrix[x][y];
		}
	}

	matrix_load(sub_A, sub_A_val);
	return sub_A;
}

matrix_t *matrix_splice(matrix_t *A, char s_dir, int s_start, int s_end) {
	if (s_start >= s_end)
		return NULL;

	if (s_dir = 'c')
		return matrix_splice_col(A, s_start, s_end);
	else if (s_dir == 'r')
		return matrix_splice_row(A, s_start, s_end);
	else
		return NULL;
}

int matrix_swap_col(matrix_t *A, int c1, int c2) {
	float *buffer = A->matrix[c1];
	A->matrix[c1] = A->matrix[c2];
	A->matrix[c2] = buffer;

	return 0;
}

int matrix_swap_row(matrix_t *A, int r1, int r2) {
	float buffer = 0;

	for (int x = 0; x < A->width; x++) {
		buffer = A->matrix[x][r1];
		A->matrix[x][r1] = A->matrix[x][r2];
		A->matrix[x][r2] = buffer;
	}

	return 0;
}

/*
	loops through matrix A (assumes width == height) and utilizes
	gauss-jordan elimination to create and return the inverse matrix
*/
matrix_t *matrix_inverse(matrix_t *A) {
	if (A->width != A->height)
		return NULL;

	// compute identity matrix of size A->width
	matrix_t *I = matrix_identity(A->width);

	// merge A and I to create the augmented matrix
	matrix_t *A_m = matrix_merge(A, I, 'c');
	matrix_print(A_m);

	// **TODO: ensure matrix has all numbers on diagonal by using
	// the pivot operation to align the matrix like:
	/*
		3 3 0
		0 3 0
		3 2 3

		instead of:

		0 3 0
		3 3 0
		3 2 3
	*/

	// start going through A_m and compute row reductions
	for (int x = 0; x < A->width; x++) {

		float x_x_val = A->matrix[x][x];
		printf("%1.5f --> %1.5f\n", x_x_val, 1.0 / x_x_val);
		// first multiply row x by 1/x_x_val
		matrix_multiply_scaler_row(A_m, 1.0 / x_x_val, x);

		for (int y = 0; y < A->height; y++) {
			if (y == x)
				continue;

			float x_y_val = A->matrix[x][y];
			if (x_y_val == 0)
				continue;
			// look at x_y_val and consider how to utlize
			// x_x_val to make x_y_val 0
			float *row = matrix_get_row(A_m, y);

			// scale row to be row * x_y_val:
			// assumes all values to left of x have already become 0
			for (int scale_row = x; scale_row < A->width; scale_row++)
				row[scale_row] *= -1 * x_y_val;

			// add row from A->matrix row y
			matrix_add_row(A_m, row, y);
			free(row);
		}
	}

	matrix_print(A_m);

	// slice out the portion that was previously the idenitity matrix,
	// this will have the inversed values within it
	matrix_t *A_i = matrix_splice(A_m, 'c', A->width, A->width * 2);

	// cleanup messes
	matrix_destroy(I);
	matrix_destroy(A_m);

	return A_i;
}

int matrix_print(matrix_t *m) {
	int cols = m->width;

	printf("|");
	for (int add_dash = 0; add_dash < cols; add_dash++)
		printf("-------%c", add_dash != cols - 1 ? '-' : '|');
	printf("\n");


	int title_len = (cols * 8) - 16;
	printf("|");
	for (int add_title = 0; add_title < title_len * 0.5; add_title++)
		printf(" ");
	printf("(%d)x(%d) Matrix", m->width, m->height);
	for (int add_title = 0; add_title < title_len * 0.5; add_title++)
		printf(" ");
	printf(" |\n");

	printf("|");
	for (int add_dash = 0; add_dash < cols; add_dash++)
		printf("-------%c", add_dash != cols - 1 ? '-' : '|');
	printf("\n");

	for (int y = 0; y < m->height; y++) {

		printf("|");
		for (int x = 0; x < m->width; x++) {
			printf(" %01.7f |", m->matrix[x][y]);
		}
		printf("\n");
	}

	printf("|");
	for (int add_dash = 0; add_dash < cols; add_dash++)
		printf("-------%c", add_dash != cols - 1 ? '-' : '|');
	printf("\n");

	return 0;
}

/*
	matrix_destroy
		takes in matrix_t *m and frees its contents
		the char f param decides if the float ** will also
		be free. Use 'f' to free
*/
int matrix_destroy(matrix_t *m, ...) {
	va_list float_free_check;
	va_start(float_free_check, m);

	char f = (char) va_arg(float_free_check, int);

	if (f == 'f') {
		for (int free_x = 0; free_x < m->width; free_x++) {
			free(m->matrix[free_x]);
		}

		free(m->matrix);
	}

	free(m);

	return 0;
}


int partial_derivative(int n, matrix_t *x, matrix_t *y, matrix_t *h_theta, matrix_t *d_h_theta) {

	// compute xXh_theta
	matrix_t *prediction = matrix_multiply(x, h_theta);

	// first comput the sum of:
	// (estimated - y) * m1_j
	// for all values i = 0 -> n
	// for each weight value v in d_h_theta
	matrix_t *err_pred = matrix_subtract(y, prediction, "-n");

	float sum;
	for (int j = 0; j < d_h_theta->height; j++) {
		sum = 0;

		for (int i = 0; i < n; i++) {
			// compute error of estimated versus y
			float error = x->matrix[i][j] * err_pred->matrix[0][i];

			sum += error;
		}

		d_h_theta->matrix[0][j] = sum * -1;
	}

	return 0;
}
/*
	Matrix Gradient Descent:

		Takes in m1, which is a fully formed mxn matrix,
	h_theta, which is an empty 1xn matrix, and y, which is
	the desired 1xn output. The algorithm continuously trys to move the
	values of h_theta to get the output of m1Xh_thetas as close
	to y as possible

	At the end, h_theta will have the results within it.
*/
int matrix_gradient_descent(matrix_t *x, matrix_t *h_theta, matrix_t *y) {
	for (int set = 0; set < h_theta->height; set++)
		h_theta->matrix[0][set] = 0;

	int n = x->width;
	float learning_rate = 0.0005;

	if (n != h_theta->height)
		return 1;

	// d_h_theta for finding partial derivative:
	matrix_t *d_h_theta = matrix_build(1, h_theta->height);
	int iteration_max = 1000, iter = 0;
	while (iter < iteration_max) {
		partial_derivative(n, x, y, h_theta, d_h_theta);
		// reconfigure h_theta based on the results of d_h_theta
		for (int h_theta_y = 0; h_theta_y < h_theta->height; h_theta_y++) {
			h_theta->matrix[0][h_theta_y] -= learning_rate * (d_h_theta->matrix[0][h_theta_y] / n);
		}

		// printf("POST ITER\n");
		// matrix_print(h_theta);

		iter++;
	}

	return 0;
}

int matrix_least_squares(matrix_t *x, matrix_t *h_theta, matrix_t *y) {
	for (int set = 0; set < h_theta->height; set++)
		h_theta->matrix[0][set] = 0;

	int n = x->width;
	float learning_rate = 0.0005;

	if (n != h_theta->height)
		return 1;

	// d_h_theta for finding partial derivative:
	matrix_t *d_h_theta = matrix_build(1, h_theta->height);
	int iteration_max = 1000, iter = 0;
	while (iter < iteration_max) {
		partial_derivative(n, x, y, h_theta, d_h_theta);
		// reconfigure h_theta based on the results of d_h_theta
		for (int h_theta_y = 0; h_theta_y < h_theta->height; h_theta_y++) {
			h_theta->matrix[0][h_theta_y] -= learning_rate * (d_h_theta->matrix[0][h_theta_y] / n);
		}

		iter++;
	}

	return 0;
}