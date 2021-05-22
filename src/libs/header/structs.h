#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define ROW 5
#define COL 3

typedef struct{
    int rows; // number of rows
    int cols; // number of columns
    int** data; // <- note that this is a pointer to one dim array
}Matrix;


typedef struct{
    int height;
    int width;
    int zSize;

    int* data;
}CirMatrix;


typedef struct{
    SDL_Surface* frame;
    SDL_Surface* conv;
    SDL_Surface* dilate;
    SDL_Surface* eroded;
}ThreadD;


typedef struct{
    int x ;
    int y ;
    int val_max;
}Candidates;


typedef struct{
    SDL_Surface* fusion_bin;
    int R;
    Candidates** arr;
}ThreadDC;

Candidates* new_candidate();

CirMatrix* new_cir_matrix(int width, int height,int zSize);

int offset(int x, int y, int z, CirMatrix* matrix);

void print_cir_matrix(CirMatrix* matrix);

void free_cir_matrix(CirMatrix* matrix);

Matrix* new_matrix(int n_rows, int n_cols);

void free_matrix(Matrix* m);

void print_matrix(Matrix* m);

#endif
