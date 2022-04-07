#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

int gradient_test() {
	matrix_t *documents = matrix_build(3, 3);
	float **doc_values = malloc(sizeof(float *) * 3);
	doc_values[0] = malloc(sizeof(float) * 3);
	doc_values[0][0] = 5;
	doc_values[0][1] = 0.1;
	doc_values[0][2] = 0.102;

	doc_values[1] = malloc(sizeof(float) * 3);
	doc_values[1][0] = 3;
	doc_values[1][1] = 0.05;
	doc_values[1][2] = 0.07;

	doc_values[2] = malloc(sizeof(float) * 3);
	doc_values[2][0] = 1;
	doc_values[2][1] = 0.09;
	doc_values[2][2] = 0.1;

	matrix_load(documents, doc_values);

	matrix_t *test_output = matrix_build(3, 1);

	matrix_print(test_output);

	matrix_t *expected_output = matrix_build(3, 1);
	float **expected_values = malloc(sizeof(float *) * 3);
	expected_values[0] = malloc(sizeof(float));
	expected_values[0][0] = 1;
	expected_values[1] = malloc(sizeof(float));
	expected_values[1][0] = 0.1;
	expected_values[2] = malloc(sizeof(float));
	expected_values[2][0] = -1;
	matrix_load(expected_output, expected_values);

	matrix_gradient_descent(documents, test_output, expected_output);

	matrix_print(test_output);
}

int main() {
	gradient_test();

	printf("TESTS PASSED\n");

	return 0;
}