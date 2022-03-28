/* 
 * File:   MatrixMath.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on January 13, 2022, 4:54 PM
 */

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// User libraries
#include "BOARD.h"
#include "MatrixMath.h"

// **** Declared function prototypes ****
void MatrixPrint(float mat[3][3]);
int MatrixEquals(float mat1[3][3], float mat2[3][3]);
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]);
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]);
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]);
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]);
float MatrixTrace(float mat[3][3]);
void MatrixTranspose(float mat[3][3], float result[3][3]);
void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]);
float MatrixDeterminant(float mat[3][3]);
float MatrixDeterminant2x2(float mat[2][2]);
void MatrixInverse(float mat[3][3], float result[3][3]);

/********************************************************************************
 * Defined MatrixPrint function.
 ********************************************************************************/
void MatrixPrint(float mat[3][3]) {
    printf(" ________________________________ \n");
    for (int r = 0; r < 3; r++) {
        printf("|");
        for (int c = 0; c < 3; c++) {
            printf(" %f |", mat[r][c]);
        }
        printf("\n -------------------------------- \n");
    }
}

/********************************************************************************
 * Defined MatrixEquals function.
 ********************************************************************************/
int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    int count;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (abs(mat1[r][c] - mat2[r][c]) <= FP_DELTA) {
            } else {
                count = (char) 'F';
                return count;
            }
        }
    }
    count = (char) 'T';
    return count;
}

/********************************************************************************
 * Defined MatrixAdd function.
 ********************************************************************************/
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            result[r][c] = mat1[r][c] + mat2[r][c];
        }
    }
}

/********************************************************************************
 * Defined MatrixMultiply function.
 ********************************************************************************/
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            result[r][c] = mat1[r][c] * mat2[r][c];
        }
    }
}

/********************************************************************************
 * Defined MatrixScalarAdd function.
 ********************************************************************************/
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            result[r][c] = mat[r][c] + x;
        }
    }
}

/********************************************************************************
 * Defined MatrixScalarMultiply function.
 ********************************************************************************/
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            result[r][c] = mat[r][c] * x;
        }
    }
}

/********************************************************************************
 * Defined MatrixTrace function.
 ********************************************************************************/
float MatrixTrace(float mat[3][3]) {
    float trace = 0;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (r == c) {
                trace += mat[r][c];
            }
        }
    }
    return trace;
}

/********************************************************************************
 * Defined MatrixTranspose function.
 ********************************************************************************/
void MatrixTranspose(float mat[3][3], float result[3][3]) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            result[c][r] = mat[r][c];
        }
    }
}

/********************************************************************************
 * Defined MatrixSubmatrix function.
 ********************************************************************************/
void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]) {
    int row = 0;
    int col = 0;
    for (int r = 0; r < 3; r++) {
        if (r == i) {
            continue;
        }
        for (int c = 0; c < 3; c++) {
            if (c == j) {
                continue;
            } else {
                if ((col == 2) && (row == 0)) {
                    col = 0;
                    row++;
                    result[row][col] = mat[r][c];
                } else if ((col == 1) && (row == 1)) {
                    result[row][col] = mat[r][c];
                    break;
                } else if ((col == 0) && (row == 1)) {
                    col++;
                    result[row][col] = mat[r][c];
                } else {
                    result[row][col] = mat[r][c];
                    col++;
                }
            }
        }
    }
}

/********************************************************************************
 * Defined MatrixDeterminant function.
 ********************************************************************************/
float MatrixDeterminant(float mat[3][3]) {
    float result1[2][2];
    float result2[2][2];
    float result3[2][2];
    MatrixSubmatrix(0, 0, mat, result1);
    MatrixSubmatrix(0, 1, mat, result2);
    MatrixSubmatrix(0, 2, mat, result3);
    return ((mat[0][0] * MatrixDeterminant2x2(result1))
            - (mat[0][1] * MatrixDeterminant2x2(result2))
            + (mat[0][2] * MatrixDeterminant2x2(result3)));
}

/********************************************************************************
 * Defined MatrixDeterminant2x2 helper function.
 ********************************************************************************/
float MatrixDeterminant2x2(float mat[2][2]) {
    return ((mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]));
}

/********************************************************************************
 * Defined MatrixInverse function.
 ********************************************************************************/
void MatrixInverse(float mat[3][3], float result[3][3]) {
    float det = MatrixDeterminant(mat);
    float cofactor[3][3];
    float subMat[2][2];
    float transpose[3][3];
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (((r + c) == 0) || ((r + c) == 2) || ((r + c) == 4)) {
                MatrixSubmatrix(r, c, mat, subMat);
                cofactor[r][c] = MatrixDeterminant2x2(subMat);
            } else {
                MatrixSubmatrix(r, c, mat, subMat);
                cofactor[r][c] = -1 * MatrixDeterminant2x2(subMat);
            }
        }
    }
    MatrixTranspose(cofactor, transpose);
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            result[r][c] = transpose[r][c] / det;
        }
    }
}