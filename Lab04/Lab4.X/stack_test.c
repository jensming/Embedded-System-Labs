// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "stack.h"

int main() {
    BOARD_Init();

    printf("\n###### Beginning 1725314's stack test harness: ####\n\n");

    // Tests for StackInit()
#if 1
    printf("Does StackInit() set the currentItemIndex appropriately?\n");
    struct Stack stack = {};
    StackInit(&stack);
    if (stack.currentItemIndex == -1) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    printf("Does StackInit() initialize the stack appropriately?\n");
    StackInit(&stack);
    if (stack.initialized == TRUE) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
#endif

    // Tests for StackPush()
#if 0
    printf("Does StackPush() push items onto the stack?\n");
    struct Stack stack = {};
    StackInit(&stack);
    if (StackPush(&stack, 2.5) == SUCCESS) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    printf("Does StackPush() detect when the stack is full?\n");
    StackInit(&stack);
    for (int i = 0; i < STACK_SIZE; i++) {
        StackPush(&stack, 34.2);
    }
    if (StackPush(&stack, -3.61) == STANDARD_ERROR) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
#endif

    // Tests for StackPop()
#if 0
    printf("Does StackPop() pop items from the stack?\n");
    struct Stack stack = {};
    double value;
    StackInit(&stack);
    StackPush(&stack, 6.044);
    if ((StackPop(&stack, &value) == SUCCESS) && (value == 6.044)) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    printf("Does StackPop() detect when the stack is empty?\n");
    StackInit(&stack);
    if (StackPop(&stack, &value) == STANDARD_ERROR) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
#endif

    // Tests for StackIsEmpty()
#if 0
    printf("Does StackIsEmpty() detect when the stack is empty?\n");
    struct Stack stack = {};
    StackInit(&stack);
    if (StackIsEmpty(&stack) == TRUE) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    printf("Does StackIsEmpty() detect when the stack isn't empty?\n");
    StackInit(&stack);
    StackPush(&stack, 53);
    if (StackIsEmpty(&stack) == FALSE) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
#endif

    // Tests for StackIsFull()
#if 0
    printf("Does StackIsFull() detect when the stack is full?\n");
    struct Stack stack = {};
    StackInit(&stack);
    for (int i = 0; i < STACK_SIZE; i++) {
        StackPush(&stack, 0.32);
    }
    if (StackIsFull(&stack) == TRUE) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    printf("Does StackIsFull() detect when the stack isn't full?\n");
    StackInit(&stack);
    StackPush(&stack, 53);
    if (StackIsFull(&stack) == FALSE) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
#endif

    // Tests for StackGetSize()
#if 0
    printf("Does StackGetSize() detect if the stack has been initialized?\n");
    struct Stack stack = {};
    if (StackGetSize(&stack) == SIZE_ERROR) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    printf("Does StackGetSize() get the current size of active elements in the stack?\n");
    StackInit(&stack);
    for (int i = 0; i < (STACK_SIZE - 3); i++) {
        StackPush(&stack, 23);
    }
    if (StackGetSize(&stack) == 17) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
#endif

    BOARD_End();
    //test stackInit:
    while (1);
    return 0;
}