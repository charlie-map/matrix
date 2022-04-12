#ifndef __MATRIX_L__
#define __MATRIX_L__

typedef struct Matrix matrix_t;

matrix_t *matrix_build(int width, int height);
matrix_t *matrix_identity(int n);
int matrix_load(matrix_t *m, float **input);

matrix_t *matrix_add(matrix_t *m1, matrix_t *m2, char *adder_p);
matrix_t *matrix_subtract(matrix_t *m1, matrix_t *m2, char *subber_p);

matrix_t *matrix_transpose(matrix_t *m);
matrix_t *matrix_multiply(matrix_t *m1, matrix_t *m2);

int matrix_svd(matrix_t *A, matrix_t *U, matrix_t *V_t, matrix_t *Sigma);
matrix_t *matrix_inverse(matrix_t *A);

int matrix_print(matrix_t *m);

int matrix_gradient_descent(matrix_t *x, matrix_t *h_theta, matrix_t *y);
int matrix_least_squares(matrix_t *x, matrix_t *h_theta, matrix_t *y);

int matrix_destroy(matrix_t *m, ...);

#endif /* __MATRIX_L__ */