#ifndef __MATRIX_L__
#define __MATRIX_L__

typedef struct Matrix matrix_t;

matrix_t *matrix_build(int dimension_num, ...);

int matrix_update(matrix_t *m, ...);

matrix_t *matrix_add(matrix_t *m1, matrix_t *m2);
matrix_t *matrix_subtract(matrix_t *m1, matrix_t *m2);

matrix_t *matrix_multiply(matrix_t *m1, matrix_t *m2);
matrix_t *matrix_divide(matrix_t *m1, matrix_t *m2);

#endif /* __MATRIX_L__ */