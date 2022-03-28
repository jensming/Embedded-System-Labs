// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

int main() {
    BOARD_Init();

    printf("\n###### Beginning 1725314's rpn test harness: ####\n\n");

#if 1
    int error;

    char test0[] = "1 1 +";
    double result0;
    double expected0 = 2;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test0);
    error = RPN_Evaluate(test0, &result0);
    if (error) {
        printf("    Failed, RPN_Evaluate produced an error\n");
    } else if (result0 != expected0) {
        printf("    Failed, expected = %f , result = %f\n", expected0, result0);
    } else {
        printf("    Success!\n");
    }

    char test1[] = "2.3 -4 -";
    double result1;
    double expected1 = 6.3;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test1);
    error = RPN_Evaluate(test1, &result1);
    if (error) {
        printf("    Failed, RPN_Evaluate produced an error\n");
    } else if (result1 != expected1) {
        printf("    Failed, expected = %f , result = %f\n", expected1, result1);
    } else {
        printf("    Success!\n");
    }

    char test2[] = "6 10 2 * /";
    double result2;
    double expected2 = 0.3;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test2);
    error = RPN_Evaluate(test2, &result2);
    if (error) {
        printf("    Failed, RPN_Evaluate produced an error\n");
    } else if (result2 != expected2) {
        printf("    Failed, expected = %f , result = %f\n", expected2, result2);
    } else {
        printf("    Success!\n");
    }

    char test3[] = "7 0 /";
    double result3;
    double nonExpected3 = 0;
    printf("Testing RPN_Evaluate with \"%s\" for a divide-by-zero error... \n", test3);
    error = RPN_Evaluate(test3, &result3);
    if (error) {
        printf("    Success! RPN_Evaluate produced the expected error\n");
    } else if (result3 == nonExpected3) {
        printf("    Failed, nonExpected = %f , result = %f which is incorrect\n", nonExpected3, result3);
    } else {
        printf("    Failed, RPN_Evaluate produced an unexpected error\n");
    }

    char test4[] = "1 1 1 1 + + +";
    double result4;
    double expected4 = 4;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test4);
    error = RPN_Evaluate(test4, &result4);
    if (error) {
        printf("    Failed, RPN_Evaluate produced an error\n");
    } else if (result4 != expected4) {
        printf("    Failed, expected = %f , result = %f\n", expected4, result4);
    } else {
        printf("    Success!\n");
    }

    char test5[] = "-1 -1 + -1 -1 + +";
    double result5;
    double expected5 = -4;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test5);
    error = RPN_Evaluate(test5, &result5);
    if (error) {
        printf("    Failed, RPN_Evaluate produced an error\n");
    } else if (result5 != expected5) {
        printf("    Failed, expected = %f , result = %f\n", expected5, result5);
    } else {
        printf("    Success!\n");
    }

    char test6[] = "1 4 + 6 4 - * 8 /";
    double result6;
    double expected6 = 1.25;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test6);
    error = RPN_Evaluate(test6, &result6);
    if (error) {
        printf("    Failed, RPN_Evaluate produced an error\n");
    } else if (result6 != expected6) {
        printf("    Failed, expected = %f , result = %f\n", expected6, result6);
    } else {
        printf("    Success!\n");
    }

    char test7[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21";
    double result7;
    printf("Testing RPN_Evaluate with \"%s\" for a stack overflow error... \n", test7);
    error = RPN_Evaluate(test7, &result7);
    if (error) {
        printf("    Success! RPN_Evaluate produced the expected error\n");
    } else {
        printf("    Failed, RPN_Evaluate produced an unexpected error\n");
    }

    char test8[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 + + + + + + + + + + + + + + + + + + +";
    double result8;
    double expected8 = 210;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test8);
    error = RPN_Evaluate(test8, &result8);
    if (error) {
        printf("    Failed, RPN_Evaluate produced an error\n");
    } else if (result8 != expected8) {
        printf("    Failed, expected = %f , result = %f\n", expected8, result8);
    } else {
        printf("    Success!\n");
    }

    char test9[] = "-8.934 *";
    double result9;
    printf("Testing RPN_Evaluate with \"%s\" for lack of operands before operator error... \n", test9);
    error = RPN_Evaluate(test9, &result9);
    if (error) {
        printf("    Success! RPN_Evaluate produced the expected error\n");
    } else {
        printf("    Failed, RPN_Evaluate produced an unexpected error\n");
    }

    char test10[] = "6 a 3.4 error / +";
    double result10;
    printf("Testing RPN_Evaluate with \"%s\" for invalid operand error... \n", test10);
    error = RPN_Evaluate(test10, &result10);
    if (error) {
        printf("    Success! RPN_Evaluate produced the expected error\n");
    } else {
        printf("    Failed, RPN_Evaluate produced an unexpected error\n");
    }

    char test11[] = "1 3 5 +";
    double result11;
    printf("Testing RPN_Evaluate with \"%s\" for too many operands in stack error... \n", test11);
    error = RPN_Evaluate(test11, &result11);
    if (error) {
        printf("    Success! RPN_Evaluate produced the expected error\n");
    } else {
        printf("    Failed, RPN_Evaluate produced an unexpected error\n");
    }

    char test12[] = "7 9 + - * /";
    double result12;
    printf("Testing RPN_Evaluate with \"%s\" for too many operators in string error... \n", test12);
    error = RPN_Evaluate(test12, &result12);
    if (error) {
        printf("    Success! RPN_Evaluate produced the expected error\n");
    } else {
        printf("    Failed, RPN_Evaluate produced an unexpected error\n");
    }

    char test13[] = "\n";
    double result13;
    printf("Testing RPN_Evaluate with \"%s\" for no input... \n", test13);
    error = RPN_Evaluate(test13, &result13);
    if (error) {
        printf("    Success! RPN_Evaluate produced the expected error\n");
    } else {
        printf("    Failed, RPN_Evaluate produced an unexpected error\n");
    }
#endif

#if 0
    printf("Testing ProcessBackspaces:\n");
    char test_pb1[] = "123\b34";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("should be : 1234\n");

    printf("Testing ProcessBackspaces:\n");
    char test_pb2[] = "123\b\b34";
    ProcessBackspaces(test_pb2);
    printf("result    : %s\n", test_pb2);
    printf("should be : 134\n");

    printf("Testing ProcessBackspaces:\n");
    char test_pb3[] = "12334\b";
    ProcessBackspaces(test_pb3);
    printf("result    : %s\n", test_pb3);
    printf("should be : 1233\n");

    printf("Testing ProcessBackspaces:\n");
    char test_pb4[] = "\b12334";
    ProcessBackspaces(test_pb4);
    printf("result    : %s\n", test_pb4);
    printf("should be : 12334\n");

    printf("Testing ProcessBackspaces:\n");
    char test_pb5[] = "\b12\b\b3\b3\b4\b\b";
    ProcessBackspaces(test_pb5);
    printf("result    : %s\n", test_pb5);
    printf("should be : \n");
#endif

    BOARD_End();
    while (1);
}