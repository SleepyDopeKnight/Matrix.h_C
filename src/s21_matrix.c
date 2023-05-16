#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
    int res = 0;
    if (rows > 0 && columns > 0) {
        result->columns = columns;
        result->rows = rows;
        if ((result->matrix = (double **)calloc(rows, sizeof(double *))) == NULL) {
            res = 1;
        } else {
            for (int i = 0; i < rows; i++) {
                if (((result->matrix[i] = (double *)calloc(columns, sizeof(double))) == NULL)) {
                    for (int j = 0; j < i; j++) free(result->matrix[j]);
                    free(result->matrix);
                    res = 1;
                }
            }
        }
    } else {
        res = 1;
    }
    return res;
}

void s21_remove_matrix(matrix_t *A) {
    if (A->matrix) {
        for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
        free(A->matrix);
        A->matrix = NULL;
    }
    A->rows = 0;
    A->columns = 0;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
    int res = SUCCESS;
    if (A->matrix && B->matrix && equal(A, B)) {
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
                    res = FAILURE;
                    break;
                }
            }
        }
    } else {
        res = FAILURE;
    }
    return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = 0;
    if (!exist(A) && !exist(B)) {
        res = 1;
    } else if (!equal(A, B)) {
        res = 2;
    } else {
        s21_create_matrix(A->rows, A->columns, result);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
            }
        }
    }
    return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = 0;
    if (!exist(A) && !exist(B)) {
        res = 1;
    } else if (!equal(A, B)) {
        res = 2;
    } else {
        s21_create_matrix(A->rows, A->columns, result);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
            }
        }
    }
    return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
    int res = 0;
    if (exist(A)) {
        if (s21_create_matrix(A->rows, A->columns, result)) {
            res = 2;
        } else {
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < A->columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] * number;
                }
            }
        }
    } else {
        res = 1;
    }
    return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = 0;
    if (exist(A) && exist(B)) {
        if (A->columns != B->rows) {
            res = 2;
        } else {
            s21_create_matrix(A->rows, B->columns, result);
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < B->columns; j++) {
                    for (int k = 0; k < A->columns; k++) {
                        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
                    }
                }
            }
        }
    } else {
        res = 1;
    }
    return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
    int res = 0;
    if (!exist(A)) {
        res = 1;
    } else {
        int col = A->rows;
        int row = A->columns;
        s21_create_matrix(row, col, result);
        for (int i = 0; i < result->rows; i++) {
            for (int j = 0; j < result->columns; j++) {
                result->matrix[i][j] = A->matrix[j][i];
            }
        }
    }
    return res;
}

void s21_mini(int n, int c, matrix_t *A, matrix_t *mini) {
    s21_create_matrix(A->rows - 1, A->columns - 1, mini);
    int i_m = 0;
    for (int i = 0; i < A->rows; i++) {
        int j_m = 0;
        if (i == n) continue;
        for (int j = 0; j < A->columns; j++) {
            if (j != c) {
                mini->matrix[i_m][j_m] = A->matrix[i][j];
                j_m++;
            }
        }
        i_m++;
    }
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
    int res = 0;
    if (!exist(A)) {
        res = 1;
    } else {
        int col = A->rows;
        int row = A->columns;
        s21_create_matrix(row, col, result);
        for (int i = 0; i < result->rows; i++) {
            for (int j = 0; j < result->columns; j++) {
                matrix_t minor_mat;
                s21_mini(i, j, A, &minor_mat);
                double r;
                s21_determinant(&minor_mat, &r);
                result->matrix[i][j] = pow((-1), i + j) * r;
                s21_remove_matrix(&minor_mat);
            }
        }
    }
    return res;
}

double s21_det(int n, matrix_t *A) {
    matrix_t submat;
    s21_create_matrix(A->rows, A->columns, &submat);
    double r = 0;
    if (n == 2) {
        r = A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
    } else {
        for (int c = 0; c < n; c++) {
            int subi = 0;
            for (int i = 1; i < n; i++) {
                int subj = 0;
                for (int j = 0; j < n; j++) {
                    if (j != c) {
                        submat.matrix[subi][subj] = A->matrix[i][j];
                        subj++;
                    }
                }
                subi++;
            }
            r += (pow(-1, c) * A->matrix[0][c] * s21_det(n - 1, &submat));
        }
    }
    s21_remove_matrix(&submat);
    return r;
}

int s21_determinant(matrix_t *A, double *result) {
    int res = 0;
    if (!exist(A)) {
        res = 1;
    } else if (A->columns != A->rows) {
        res = 2;
    } else {
        if (A->rows == 1) {
            *result = A->matrix[0][0];
        } else {
            *result = s21_det(A->rows, A);
        }
    }
    return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
    int res = 0;
    double r;
    if (!exist(A)) {
        res = 1;
    } else if (A->columns != A->rows) {
        res = 2;
    } else if (!s21_determinant(A, &r) && !r) {
        res = 2;
    } else {
        matrix_t complements, transpose;
        s21_calc_complements(A, &complements);
        s21_transpose(&complements, &transpose);
        int col = A->rows;
        int row = A->columns;
        s21_create_matrix(row, col, result);
        for (int i = 0; i < result->rows; i++) {
            for (int j = 0; j < result->columns; j++) {
                result->matrix[i][j] = 1/r * transpose.matrix[i][j];
            }
        }
        s21_remove_matrix(&complements);
        s21_remove_matrix(&transpose);
    }
    return res;
}

int exist(matrix_t *A) {
    int res = 0;
    if (A != NULL && A->rows > 0 && A->columns > 0 && A->matrix != NULL) res = 1;
    return res;
}

int equal(matrix_t *A, matrix_t *B) {
    int res = 0;
    if (exist(A) && exist(B) && A->rows == B->rows && A->columns == B->columns) res = 1;
    return res;
}
