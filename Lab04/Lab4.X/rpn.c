/* 
 * File:   stack.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on January 21, 2022, 1:22 PM
 */

// Standard libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// User libraries
#include "BOARD.h"
#include "rpn.h"
#include "stack.h"

// **** Declared function prototypes ****
int RPN_Evaluate(char * rpn_string, double * result);
int ProcessBackspaces(char *rpn_sentence);

/********************************************************************************
 * Defined RPN_Evaluate function.
 ********************************************************************************/
int RPN_Evaluate(char * rpn_string, double * result) {
    struct Stack stack = {};
    StackInit(&stack);
    double operand1, operand2;
    double value; // item pushed and or popped from stack
    int isNum, pushTokenCount = 0, decimalCount = 0, signCount = 0;
    const char delim[] = " ";
    char *token = strtok(rpn_string, delim);
    while (token != NULL) {
//        printf("%s\n", token);

        // parses each token to check for appropriate elements
        for (int i = 0; i < strlen(token); i++) {
            if (isdigit(token[i])) // checks if token is a digit
            {
                isNum = 1;
            } else if (((token[i] == '-') || (token[i] == '+')) && (signCount < 2)) // checks for appropriate sign of number occurrences
            {
                signCount++;
                if (signCount > 1) {
//                    printf("invalid token! sign:%d dec:%d\n", signCount, decimalCount);
                    return RPN_ERROR_INVALID_TOKEN;
                } else {
                    continue;
                }
            } else if ((token[i] == '.') && (decimalCount < 2)) // checks for appropriate decimal point occurrences for numbers
            {
                decimalCount++;
                if (decimalCount > 1) {
//                    printf("invalid token! sign:%d dec:%d\n", signCount, decimalCount);
                    return RPN_ERROR_INVALID_TOKEN;
                } else {
                    continue;
                }
            } else if (token[i + 1] == '\0') // checks for error case when no operators are entered
            {
                break;
            } else {
                isNum = 0;
                break;
            }
        }

        if (isNum == 1) // checks if token is numeric
        {
            if (StackIsFull(&stack) == TRUE) {
//                printf("stack overflow!\n");
                return RPN_ERROR_STACK_OVERFLOW;
            } else {
                StackPush(&stack, atof(token));
                pushTokenCount++;
            }
        } else if ((token[0] == '+') || (token[0] == '-') || (token[0] == '*') || (token[0] == '/')) // checks if token is an operator
        {
            if (StackGetSize(&stack) < 2) {
//                printf("stack underflow!\n");
                return RPN_ERROR_STACK_UNDERFLOW;
            } else {
                StackPop(&stack, &value);
                operand1 = value;
                StackPop(&stack, &value);
                operand2 = value;
                if (token[0] == '+') // checks for '+' operator
                {
                    value = operand2 + operand1;
                    StackPush(&stack, value);
//                    printf("val:%f\n", value);
                } else if (token[0] == '-') // checks for '-' operator
                {
                    value = operand2 - operand1;
                    StackPush(&stack, value);
//                    printf("val:%f\n", value);
                } else if (token[0] == '*') // checks for '*' operator
                {
                    value = operand2 * operand1;
                    StackPush(&stack, value);
//                    printf("val:%f\n", value);
                } else if (token[0] == '/') // checks for '/' operator
                {
                    if (operand1 == 0) {
                        return RPN_ERROR_DIVIDE_BY_ZERO;
                    } else {
                        value = operand2 / operand1;
                        StackPush(&stack, value);
//                        printf("val:%f\n", value);
                    }
                }
            }
        } else {
//            printf("invalid token here!\n");
            return RPN_ERROR_INVALID_TOKEN;
        }
        isNum = 0;
        decimalCount = 0;
        signCount = 0;
        token = strtok(NULL, delim); // resets the token
    }

    //checks for too many or few items leftover in the stack
    if (StackIsEmpty(&stack) || (pushTokenCount < 2)) {
//        printf("stack empty!\n");
        return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
    } else if ((StackGetSize(&stack) > 1) || (pushTokenCount < 2)) {
//        int size = StackGetSize(&stack);
//        printf("size:%d\n", size);
//        printf("too much in stack!\n");
        return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    } else {
        StackPop(&stack, &value);
        *result = value;
    }
    return RPN_NO_ERROR;
}

/********************************************************************************
 * Defined ProcessBackspaces function.
 ********************************************************************************/
int ProcessBackspaces(char *rpn_sentence) {
    char *temp, *final; // copies of rpn_sentence, one to parse through and the other to write to

    // parses rpn_sentence and looks for backspaces until null character is reached
    for (final = temp = rpn_sentence; *temp != '\0'; temp++) {
        if (*temp == '\b') {
            if (final != rpn_sentence) {
                final--;
            } else {
                continue;
            }
        }
        *final++ = *temp;
    }
    *final = '\0';

    return strlen(rpn_sentence);
}