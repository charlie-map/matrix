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

int matrix_multiply_scaler(matrix_t *m1, float scaler) {
	for (int x = 0; x < m1->width; x++) {
		for (int y = 0; y < m1->height; y++) {
			m1->matrix[x][y] *= scaler;
		}
	}

	return 0;
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
			printf(" %01.3f |", m->matrix[x][y]);
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
	printf("prediction\n");
	matrix_print(prediction);
	// first comput the sum of:
	// (estimated - y) * m1_j
	// for all values i = 0 -> n
	// for each weight value v in d_h_theta
	float sum;
	for (int j = 0; j < d_h_theta->height; j++) {
		sum = 0;

		for (int i = 0; i < n; i++) {
			// compute error of estimated versus y
			float error = -1 * (x->matrix[i][j] * (y->matrix[0][i] - prediction->matrix[0][i]));

			sum += error;
		}

		sum /= n;

		d_h_theta->matrix[0][j] = sum;
	}

	printf("INITIAL D_H\n");
	matrix_print(d_h_theta);
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
			h_theta->matrix[0][h_theta_y] -= learning_rate * d_h_theta->matrix[0][h_theta_y];
		}

		printf("POST ITER\n");
		matrix_print(h_theta);

		iter++;
	}

	return 0;
}