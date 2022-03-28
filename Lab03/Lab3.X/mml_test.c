// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"




// User libraries:
#include "MatrixMath.h"


// Module-level variables:
float zero_matrix[3][3] = {
    {},
    {},
    {}};
float num1_matrix[3][3] = {
    {1.10, 2.20, 3.30},
    {4.40, 5.50, 6.60},
    {7.70, 8.80, 9.90}};
float num2_matrix[3][3] = {
    {3.1005, 12.2001, 5.6001},
    {1.0007, 2.8009, 9.6003},
    {2.2004, 10.5004, 0.0007}};
float num3_matrix[3][3] = {
    {-3.15, 12.21, 5.61},
    {1.07, -2.89, 9.63},
    {2.24, 10.54, 0.07}};
float expAdd_matrix[3][3] = {
    {4.200500, 14.400100, 8.900100},
    {5.400700, 8.300900, 16.200300},
    {9.900400, 19.300400, 9.900700}};
float expScalarAdd1_matrix[3][3] = {
    {2.163000, 3.263000, 4.363000},
    {5.463000, 6.563000, 7.663000},
    {8.763000, 9.863000, 10.963000}};
float expScalarAdd2_matrix[3][3] = {
    {4.163500, 13.263100, 6.663100},
    {2.063700, 3.863900, 10.663300},
    {3.263400, 11.563400, 1.063700}};
float expMult_matrix[3][3] = {
    {3.410550, 26.840220, 18.480330},
    {4.403080, 15.404950, 63.361980},
    {16.943080, 92.403520, 0.006930}};
float expScalarMult_matrix[3][3] = {
    {-3.348450, 12.979230, 5.963430},
    {1.137410, -3.072070, 10.236690},
    {2.381120, 11.204020, 0.074410}};
float expTrans1_matrix[3][3] = {
    {3.1005, 1.0007, 2.2004},
    {12.2001, 2.8009, 10.5004},
    {5.6001, 9.6003, 0.0007}};
float expTrans2_matrix[3][3] = {
    {-3.15, 1.07, 2.24},
    {12.21, -2.89, 10.54},
    {5.61, 9.63, 0.07}};
float expSubMat1_matrix[3][3] = {
    {4.400000, 6.600000, 0.000000},
    {7.700000, 9.900000, 0.000000},
    {0.000000, 0.000000, 0.000000}};
float expSubMat2_matrix[3][3] = {
    {2.200000, 3.300000, 0.000000},
    {8.800000, 9.900000, 0.000000},
    {0.000000, 0.000000, 0.000000}};
float expInv1_matrix[3][3] = {
    {3.304752, -1.927503, -3.325545},
    {-0.692514, 0.403903, 0.792107},
    {-0.142433, 0.187239, 0.115544}};
float expInv2_matrix[3][3] = {
    {-0.149032, 0.085394, 0.196060},
    {0.031500, -0.018737, 0.053247},
    {0.026012, 0.088730, -0.005804}};
float result_matrix[3][3];
float result_subMatrix[2][2];
float temp_matrix[3][3];
float scalar = 1.063;
int i = 0;
int j = 1;

int main() {
    BOARD_Init();

    printf("Beginning 1725314's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);

    int count = 0; // tallies number of test passes for each function
    int total = 0; // total number of functions that have passed their tests


    // MatrixEquals() test
    //    printf("\nDemonstrating MatrixEquals():\n");
    int isEqual = MatrixEquals(num2_matrix, num1_matrix); // should be FALSE
    if (isEqual == 'T') {
        //        printf("TRUE\n");
    } else {
        count++;
        //        printf("FALSE\n");
    }
    //    printf("\nExpected output of MatrixEquals(): FALSE\n");
    isEqual = MatrixEquals(num2_matrix, num2_matrix); // should be TRUE
    if (isEqual == 'T') {
        count++;
        //        printf("TRUE\n");
    } else {
        //        printf("FALSE\n");
    }
    //    printf("\nExpected output of MatrixEquals(): TRUE\n");
    printf("\nPASSED (%d/2): MatrixEquals()", count); // MatrixEquals() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixAdd() test
    //    printf("\nDemonstrating MatrixAdd():\n");
    MatrixAdd(zero_matrix, num1_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, num1_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("Expected output of MatrixAdd():\n ________________________________ \n"
    //           "| 1.100000 | 2.200000 | 3.300000 |\n -------------------------------- \n"
    //           "| 4.400000 | 5.500000 | 6.600000 |\n -------------------------------- \n"
    //           "| 7.700000 | 8.800000 | 9.900000 |\n -------------------------------- \n");
    MatrixAdd(num1_matrix, num2_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expAdd_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("Expected output of MatrixAdd():\n ________________________________ \n"
    //           "| 4.200500 | 14.400100 | 8.900100 |\n -------------------------------- \n"
    //           "| 5.400700 | 8.300900 | 16.200300 |\n -------------------------------- \n"
    //           "| 9.900400 | 19.300400 | 9.900700 |\n -------------------------------- \n");
    printf("\nPASSED (%d/2): MatrixAdd()", count); // MatrixAdd() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixScalarAdd() test
    //    printf("\nDemonstrating MatrixScalarAdd():\n");
    MatrixScalarAdd(scalar, num1_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expScalarAdd1_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixScalarAdd():\n");
    //    MatrixPrint(expScalarAdd1_matrix);
    MatrixScalarAdd(scalar, num2_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expScalarAdd2_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixScalarAdd():\n");
    //    MatrixPrint(expScalarAdd2_matrix);
    printf("\nPASSED (%d/2): MatrixScalarAdd()", count); // MatrixScalarAdd() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixMultiply() test
    //    printf("\nDemonstrating MatrixMultiply():\n");
    MatrixMultiply(num1_matrix, num2_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expMult_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixMultiply():\n");
    //    MatrixPrint(expMult_matrix);
    //    printf("\nDemonstrating MatrixMultiply():\n");
    MatrixMultiply(num1_matrix, zero_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, zero_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixMultiply():\n");
    //    MatrixPrint(zero_matrix);
    printf("\nPASSED (%d/2): MatrixMultiply()", count); // MatrixMultiply() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixScalarMultiply() test
    //    printf("\nDemonstrating MatrixScalarMultiply():\n");
    MatrixScalarMultiply(scalar, num3_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expScalarMult_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixScalarMultiply():\n");
    //    MatrixPrint(expScalarMult_matrix);
    //    printf("\nDemonstrating MatrixScalarMultiply():\n");
    MatrixScalarMultiply(scalar, zero_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, zero_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixScalarMultiply():\n");
    //    MatrixPrint(zero_matrix);
    printf("\nPASSED (%d/2): MatrixScalarMultiply()", count); // MatrixScalarMultiply() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixTrace() test
    //    printf("\nDemonstrating MatrixTrace(): ");
    float trace = MatrixTrace(num1_matrix);
    //    printf("%f\n", trace);
    if (trace == 16.500000) {
        count++;
    }
    //    printf("\nExpected output of MatrixTrace(): 16.500000\n");
    //    printf("\nDemonstrating MatrixTrace(): ");
    trace = MatrixTrace(num3_matrix);
    //    printf("%f\n", trace);
    if ((signed)trace == (signed) - 5.970000) {
        count++;
    }
    //    printf("\nExpected output of MatrixTrace(): -5.970000\n");
    printf("\nPASSED (%d/2): MatrixTrace()", count); // MatrixTrace() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixTranspose() test
    //    printf("\nDemonstrating MatrixTranspose():\n");
    MatrixTranspose(num2_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expTrans1_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixTranspose():\n");
    //    MatrixPrint(expTrans1_matrix);
    //    printf("\nDemonstrating MatrixTranspose():\n");
    MatrixTranspose(num3_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expTrans2_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixTranspose():\n");
    //    MatrixPrint(expTrans2_matrix);
    printf("\nPASSED (%d/2): MatrixTranspose()", count); // MatrixTranspose() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixSubmatrix() test
    //    printf("\nDemonstrating MatrixSubmatrix():\n");
    MatrixSubmatrix(i, j, num1_matrix, result_subMatrix);
    //    printf(" _____________________ \n");
    for (int r = 0; r < 2; r++) {
        //        printf("|");
        for (int c = 0; c < 2; c++) {
            temp_matrix[r][c] = result_subMatrix[r][c];
            //            printf(" %f |", result_subMatrix[r][c]);
        }
        //        printf("\n --------------------- \n");
    }
    isEqual = MatrixEquals(temp_matrix, expSubMat1_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixSubmatrix():\n _____________________ \n"
    //           "| 4.400000 | 6.600000 |\n --------------------- \n"
    //           "| 7.700000 | 9.900000 |\n --------------------- \n");
    //    printf("\nDemonstrating MatrixSubmatrix():\n");
    MatrixSubmatrix(j, i, num1_matrix, result_subMatrix);
    //    printf(" _____________________ \n");
    for (int r = 0; r < 2; r++) {
        //        printf("|");
        for (int c = 0; c < 2; c++) {
            temp_matrix[r][c] = result_subMatrix[r][c];
            //            printf(" %f |", result_subMatrix[r][c]);
        }
        //        printf("\n --------------------- \n");
    }
    isEqual = MatrixEquals(temp_matrix, expSubMat2_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixSubmatrix():\n _____________________ \n"
    //           "| 2.200000 | 3.300000 |\n --------------------- \n"
    //           "| 8.800000 | 9.900000 |\n --------------------- \n");
    printf("\nPASSED (%d/2): MatrixSubmatrix()", count); // MatrixSubmatrix() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixDeterminant() test
    //    printf("\nDemonstrating MatrixDetermint(): ");
    float det = MatrixDeterminant(num1_matrix);
    //    printf("%f\n", det);
    if (abs(det - 0.000000) <= FP_DELTA) {
        count++;
    }
    //    printf("\nExpected output of MatrixDeterminant(): 0.000000\n");
    //    printf("\nDemonstrating MatrixDeterminant(): ");
    det = MatrixDeterminant(num3_matrix);
    //    printf("%f\n", det);
    if (abs(det - 682.418052) <= FP_DELTA) {
        count++;
    }
    //    printf("\nExpected output of MatrixDeterminant(): 682.418052\n");
    printf("\nPASSED (%d/2): MatrixDeterminant()", count); // MatrixDeterminant() test success rate
    if (count == 2) {
        total++;
    }
    count = 0;


    // MatrixInverse() test
    //    printf("\nDemonstrating MatrixInverse():\n");
    MatrixInverse(num2_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expInv1_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixInverse():\n");
    //    MatrixPrint(expInv1_matrix);
    //    printf("\nDemonstrating MatrixInverse():\n");
    MatrixInverse(num3_matrix, result_matrix);
    //    MatrixPrint(result_matrix);
    isEqual = MatrixEquals(result_matrix, expInv2_matrix);
    if (isEqual == 'T') {
        count++;
    }
    //    printf("\nExpected output of MatrixInverse():\n");
    //    MatrixPrint(expInv2_matrix);
    printf("\nPASSED (%d/2): MatrixInverse()\n---------------------------\n", count); // MatrixInverse() test success rate
    if (count == 2) {
        total++;
    }


    int percent = (total / 10) * 100;
    printf("%d out of 10 functions passed (%d%%).\n", total, percent); // total functions passed check


    // MatrixPrint() test
    printf("\nOutput of MatrixPrint():\n");
    //    MatrixPrint(zero_matrix);
    //    printf("Expected output of MatrixPrint():\n ________________________________ \n"
    //           "| 0.000000 | 0.000000 | 0.000000 |\n -------------------------------- \n"
    //           "| 0.000000 | 0.000000 | 0.000000 |\n -------------------------------- \n"
    //           "| 0.000000 | 0.000000 | 0.000000 |\n -------------------------------- \n");
    //    MatrixPrint(num1_matrix);
    //    printf("Expected output of MatrixPrint():\n ________________________________ \n"
    //           "| 1.100000 | 2.200000 | 3.300000 |\n -------------------------------- \n"
    //           "| 4.400000 | 5.500000 | 6.600000 |\n -------------------------------- \n"
    //           "| 7.700000 | 8.800000 | 9.900000 |\n -------------------------------- \n");
    MatrixPrint(num2_matrix);
    printf("Expected output of MatrixPrint():\n ________________________________ \n"
            "| 3.100500 | 12.200100 | 5.600100 |\n -------------------------------- \n"
            "| 1.000700 | 2.800900 | 9.600300 |\n -------------------------------- \n"
            "| 2.200400 | 10.500400 | 0.000700 |\n -------------------------------- \n");

    BOARD_End();
    while (1);
}