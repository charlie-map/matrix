#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "matrix.h"

int gradient_test() {

	matrix_t *documents = matrix_build(3, 4);
	float **doc_values = malloc(sizeof(float *) * 3);
	doc_values[0] = malloc(sizeof(float) * 4);
	doc_values[0][0] = 1;
	doc_values[0][1] = 3;
	doc_values[0][2] = 5;
	doc_values[0][3] = 4;

	doc_values[1] = malloc(sizeof(float) * 4);
	doc_values[1][0] = 0.07;
	doc_values[1][1] = 0.05;
	doc_values[1][2] = 0.1;
	doc_values[1][3] = 0.02;

	doc_values[2] = malloc(sizeof(float) * 4);
	doc_values[2][0] = 0.1;
	doc_values[2][1] = 0.09;
	doc_values[2][2] = 0.102;
	doc_values[2][3] = 0.04;

	matrix_load(documents, doc_values);

	matrix_t *test_output = matrix_build(1, 3);

	matrix_print(test_output);

	matrix_t *expected_output = matrix_build(1, 4);
	float **expected_values = malloc(sizeof(float *));
	expected_values[0] = malloc(sizeof(float) * 4);
	expected_values[0][0] = -1;
	expected_values[0][1] = 0.1;
	expected_values[0][2] = 1;
	expected_values[0][3] = 0.4;
	matrix_load(expected_output, expected_values);

	matrix_gradient_descent(documents, test_output, expected_output);

	matrix_print(test_output);

	// matrix_t *documents = matrix_build(3, 4);
	// float **doc_values = malloc(sizeof(float *) * 3);
	// doc_values[0] = malloc(sizeof(float) * 4);
	// doc_values[0][0] = 5;
	// doc_values[0][1] = 3;
	// doc_values[0][2] = 1;
	// doc_values[0][3] = 4;

	// doc_values[1] = malloc(sizeof(float) * 4);
	// doc_values[1][0] = 0.1;
	// doc_values[1][1] = 0.05;
	// doc_values[1][2] = 0.07;
	// doc_values[1][3] = 0.02;

	// doc_values[2] = malloc(sizeof(float) * 4);
	// doc_values[2][0] = 0.102;
	// doc_values[2][1] = 0.09;
	// doc_values[2][2] = 0.1;
	// doc_values[2][3] = 0.04;

	// matrix_load(documents, doc_values);

	// matrix_t *expected_output = matrix_build(1, 3);
	// float **expected_values = malloc(sizeof(float *));
	// expected_values[0] = malloc(sizeof(float) * 3);
	// expected_values[0][0] = 1;
	// expected_values[0][1] = 0.1;
	// expected_values[0][2] = -1;
	// matrix_load(expected_output, expected_values);

	// matrix_print(documents);
	// matrix_print(expected_output);

	// matrix_t *test = matrix_multiply(documents, expected_output);
	// printf("%d\n", test);
	// matrix_print(test);

	return 0;
}

int transpose_inverse_test() {
	matrix_t *A = matrix_build(3, 4);
	float **A_val = malloc(sizeof(float *) * 3);
	A_val[0] = malloc(sizeof(float) * 4);
	A_val[0][0] = 1;
	A_val[0][1] = 3;
	A_val[0][2] = 5;
	A_val[0][3] = 4;

	A_val[1] = malloc(sizeof(float) * 4);
	A_val[1][0] = 0.07;
	A_val[1][1] = 0.05;
	A_val[1][2] = 0.1;
	A_val[1][3] = 0.02;

	A_val[2] = malloc(sizeof(float) * 4);
	A_val[2][0] = 0.1;
	A_val[2][1] = 0.09;
	A_val[2][2] = 0.102;
	A_val[2][3] = 0.04;

	matrix_load(A, A_val);

	matrix_t *A_t = matrix_transpose(A);

	matrix_t *A_t_A = matrix_multiply(A_t, A);
	matrix_print(A_t_A);

	matrix_t *y = matrix_build(1, 4);
	float **y_val = malloc(sizeof(float *) * 1);
	y_val[0] = malloc(sizeof(float) * 4);
	y_val[0][0] = -1;
	y_val[0][1] = 0.1;
	y_val[0][2] = 1;
	y_val[0][3] = 0.4;

	matrix_load(y, y_val);

	matrix_t *A_t_y = matrix_multiply(A_t, y);

	matrix_print(A_t_y);

	matrix_t *A_t_A_i = matrix_build(3, 3);
	float **A_t_A_i_val = malloc(sizeof(float *) * 3);
	A_t_A_i_val[0] = malloc(sizeof(float) * 3);
	A_t_A_i_val[0][0] = 0.068740693676948557096;
	A_t_A_i_val[0][1] = -1.5865452049272074821;
	A_t_A_i_val[0][2] = -1.1889800130602023332;

	A_t_A_i_val[1] = malloc(sizeof(float) * 3);
	A_t_A_i_val[1][0] = -1.586545204927207486;
	A_t_A_i_val[1][1] = 1053.6034918856758556;
	A_t_A_i_val[1][2] = -732.66248851658952192;

	A_t_A_i_val[2] = malloc(sizeof(float) * 3);
	A_t_A_i_val[2][0] = -1.1889800130602023301;
	A_t_A_i_val[2][1] = -732.66248851658952205;
	A_t_A_i_val[2][2] = 621.89228026859801587;

	matrix_load(A_t_A_i, A_t_A_i_val);
	matrix_print(A_t_A_i);

	matrix_t *A_t_A_i_A_t_y = matrix_multiply(A_t_A_i, A_t_y);

	matrix_print(A_t_A_i_A_t_y);

	return 0;
}

int inverse_test() {
	// test 1:
	/*
		input:
		0 2 0
		2 2 0
		2 1 2

		output:
		-1/2  1/2  0
		 1/2   0   0
		 1/4 -1/2 1/2
	*/

	matrix_t *A = matrix_build(3, 3);
	float **A_val = malloc(sizeof(float *) * 3);
	A_val[0] = malloc(sizeof(float) * 3);
	A_val[0][0] = 2;
	A_val[0][1] = 2;
	A_val[0][2] = 0;

	A_val[1] = malloc(sizeof(float) * 3);
	A_val[1][0] = 0;
	A_val[1][1] = 2;
	A_val[1][2] = 0;

	A_val[2] = malloc(sizeof(float) * 3);
	A_val[2][0] = 2;
	A_val[2][1] = 1;
	A_val[2][2] = 2;

	matrix_load(A, A_val);

	matrix_t *A_i = matrix_inverse(A);

	matrix_print(A);
	matrix_print(A_i);

	return 0;
}

int main() {
	// gradient_test();
	// transpose_inverse_test();
	inverse_test();

	printf("TESTS PASSED\n");

	return 0;
}