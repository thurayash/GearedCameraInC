#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>

#define ROW 5
#define COL 3

typedef struct{
    int rows; // number of rows
    int cols; // number of columns
    int** data; // <- note that this is a pointer to one dim array
} Matrix;


typedef struct{
    int height;
    int width;
    int zSize;

    int* data;
}CirMatrix;



Matrix* new_matrix(int n_rows, int n_cols);

void free_matrix(Matrix* m);

void print_matrix(Matrix* m);

#endif
