#include "../header/structs.h"

CirMatrix* new_cir_matrix(int width, int height,int zSize){
    CirMatrix* new_matrix = malloc(sizeof(CirMatrix));

    new_matrix->height = height;
    new_matrix->width = width;
    new_matrix->zSize = zSize;

    new_matrix->data = calloc(height*width*zSize, sizeof(int));

    return new_matrix;
}

int offset(int x, int y, int z, CirMatrix* matrix){
    return (z * matrix->zSize * matrix->width) + (x*matrix->height) + y;
}
/*
 * [
 *  [ [0,1,2], [3,4,5]   ],
 *  [ [6,7,8], [9,10,11] ]
 * ]
 *
 * 2*2*3
 * data = [0,6,3,9,1,7,4,10,2,8,5,11]
 *
*/
void print_cir_matrix(CirMatrix* matrix){
    for(int i = 0; i < matrix->height; i++)
    {
        for(int j = 0; j < matrix->width; j++)
        {
            printf("[");
            for(int k = 0; k < matrix->zSize - 1; k++)
            {
                printf("%i,",
                    matrix->data[(k * matrix->height * matrix->width ) +
                    (j*matrix->height) + i]);
            }

            printf("%i",
                    matrix->data[((matrix->zSize-1)* matrix->height *
                        matrix->width ) +
                    (j*matrix->height) + i]);
            printf("]\t");
        }
        printf("\n");
    }
}


void free_cir_matrix(CirMatrix* matrix){
    free(matrix->data);
    free(matrix);
}


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


