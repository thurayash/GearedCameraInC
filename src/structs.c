#include "structs.h"


Matrix* new_matrix(int n_cols, int n_rows) {
    Matrix* matrix = malloc(sizeof(Matrix));
    matrix->rows = n_rows;
    matrix->cols = n_cols;
    int** data = malloc(sizeof(double*) * n_rows);
    for(int x = 0; x < n_rows; x++){
        data[x] = calloc(n_cols, sizeof(double));
    }
    matrix->data = data;
    return matrix;
}

void free_matrix(Matrix* m)
{
    for(int i = 0; i < m->rows; i++)
        free(m->data[i]);
    free(m->data);
    free(m);
}


void print_matrix(Matrix* m)
{
    printf("Matrix : %p", m);
    for(int i = 0; i != m->rows; i++)
    {
        for(int j = 0; j != m->cols; j++)
        {
            printf("%i ", m->data[i][j]);
        }
        printf("\n");
    }
    printf("--------\n");
}


