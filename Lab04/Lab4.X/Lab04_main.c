// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60

int main() {
    BOARD_Init();

    char rpn_sentence[MAX_INPUT_LENGTH + 2];
    double result;
    int error;

    printf("Welcome to 1725314'S RPN calculator.  Compiled on %s %s\n\n", __DATE__, __TIME__);
    while (1) {

        printf("Enter floats and + - / * in RPN format:\n");

        fgets(rpn_sentence, sizeof (rpn_sentence), stdin);

        ProcessBackspaces(rpn_sentence);

        error = RPN_Evaluate(rpn_sentence, &result);

        if (strlen(rpn_sentence) > MAX_INPUT_LENGTH) {
            printf("ERROR: Too many characters entered.\n\n");
            while (fgetc(stdin) != '\n') {
            }
        } else if (error == RPN_ERROR_STACK_OVERFLOW) {
            printf("ERROR: No more room on stack.\n\n");
        } else if (error == RPN_ERROR_STACK_UNDERFLOW) {
            printf("ERROR: Not enough operands before operator.\n\n");
        } else if (error == RPN_ERROR_INVALID_TOKEN) {
            printf("ERROR: Invalid character in RPN string.\n\n");
        } else if (error == RPN_ERROR_DIVIDE_BY_ZERO) {
            printf("ERROR: Infinite number from divide-by-zero.\n\n");
        } else if ((error == RPN_ERROR_TOO_FEW_ITEMS_REMAIN) || (error == RPN_ERROR_TOO_MANY_ITEMS_REMAIN)) {
            printf("ERROR: Invalid RPN calculation due to more or less than one item in the stack.\n\n");
        } else {
            printf("result = %f\n\n", result);
        }

    }

    while (1);
}