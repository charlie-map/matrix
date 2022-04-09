#ifndef __MATRIX_L__
#define __MATRIX_L__

typedef struct Matrix matrix_t;

matrix_t *matrix_build(int width, int height);
int matrix_load(matrix_t *m, float **input);

matrix_t *matrix_add(matrix_t *m1, matrix_t *m2, char *adder_p);
matrix_t *matrix_subtract(matrix_t *m1, matrix_t *m2, char *subber_p);

matrix_t *matrix_transpose(matrix_t *m);
matrix_t *matrix_multiply(matrix_t *m1, matrix_t *m2);

int matrix_print(matrix_t *m);

int matrix_gradient_descent(matrix_t *m1, matrix_t *variable, matrix_t *expected);

int matrix_destroy(matrix_t *m, ...);

#endif /* __MATRIX_L__ */