#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

int gradient_test() {

	matrix_t *documents = matrix_build(3, 4);
	float **doc_values = malloc(sizeof(float *) * 3);
	doc_values[0] = malloc(sizeof(float) * 4);
	doc_values[0][0] = 5;
	doc_values[0][1] = 3;
	doc_values[0][2] = 1;
	doc_values[0][3] = 4;

	doc_values[1] = malloc(sizeof(float) * 4);
	doc_values[1][0] = 0.1;
	doc_values[1][1] = 0.05;
	doc_values[1][2] = 0.07;
	doc_values[1][3] = 0.02;

	doc_values[2] = malloc(sizeof(float) * 4);
	doc_values[2][0] = 0.102;
	doc_values[2][1] = 0.09;
	doc_values[2][2] = 0.1;
	doc_values[2][3] = 0.04;

	matrix_load(documents, doc_values);

	matrix_t *test_output = matrix_build(1, 3);

	matrix_print(test_output);

	matrix_t *expected_output = matrix_build(1, 4);
	float **expected_values = malloc(sizeof(float *));
	expected_values[0] = malloc(sizeof(float) * 4);
	expected_values[0][0] = 1;
	expected_values[0][1] = 0.1;
	expected_values[0][2] = -1;
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

int main() {
	gradient_test();

	printf("TESTS PASSED\n");

	return 0;
}