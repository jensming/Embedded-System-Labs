/* 
 * File:   stack.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on January 21, 2022, 1:14 PM
 */

// Standard libraries
#include <stdio.h>

// User libraries
#include "BOARD.h"
#include "stack.h"

// **** Declared function prototypes ****
void StackInit(struct Stack *stack);
int StackPush(struct Stack *stack, double value);
int StackPop(struct Stack *stack, double *value);
int StackIsEmpty(const struct Stack *stack);
int StackIsFull(const struct Stack *stack);
int StackGetSize(const struct Stack *stack);

/********************************************************************************
 * Defined StackInit function.
 ********************************************************************************/
void StackInit(struct Stack *stack) {
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}

/********************************************************************************
 * Defined StackPush function.
 ********************************************************************************/
int StackPush(struct Stack *stack, double value) {
    if (stack->initialized != TRUE) {
        return STANDARD_ERROR;
    } else if (StackIsFull(stack) == TRUE) {
        return STANDARD_ERROR;
    } else {
        stack->currentItemIndex++;
        stack->stackItems[stack->currentItemIndex] = value;
        return SUCCESS;
    }
}

/********************************************************************************
 * Defined StackPop function.
 ********************************************************************************/
int StackPop(struct Stack *stack, double *value) {
    if (stack->initialized != TRUE) {
        return STANDARD_ERROR;
    } else if (StackIsEmpty(stack) == TRUE) {
        return STANDARD_ERROR;
    } else {
        *value = stack->stackItems[stack->currentItemIndex--];
        return SUCCESS;
    }
}

/********************************************************************************
 * Defined StackIsEmpty function.
 ********************************************************************************/
int StackIsEmpty(const struct Stack *stack) {
    if ((stack->currentItemIndex == -1) && (stack->initialized == TRUE)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/********************************************************************************
 * Defined StackIsFull function.
 ********************************************************************************/
int StackIsFull(const struct Stack *stack) {
    if ((stack->currentItemIndex == (STACK_SIZE - 1)) && (stack->initialized == TRUE)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/********************************************************************************
 * Defined StackGetSize function.
 ********************************************************************************/
int StackGetSize(const struct Stack *stack) {
    if (stack->initialized != TRUE) {
        return SIZE_ERROR;
    } else {
        return stack->currentItemIndex + 1;
    }
}