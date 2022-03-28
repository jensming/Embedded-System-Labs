// Standard libraries
#include <stdio.h>
#include <math.h>


// User libraries:
#include "BOARD.h"

#ifdef PIC32
// Microchip libraries
#include <xc.h>
#endif


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);
double AbsoluteValue(double operand);
double Average(double operand1, double operand2);
double FahrenheitToCelsius(double operand);
double CelsiusToFahrenheit(double operand);
double Tangent(double operand);
double DegToRad(double operand);
double Round(double operand);

void CalculatorRun(void) {
    printf("\n\nWelcome to James' calculator program! Compiled at %s %s\n", __DATE__, __TIME__);

    //    //Character prompt test
    //    char chr;
    //    while (1)
    //    {
    //        char chr;
    //        scanf(" %c", &chr);
    //        printf("Character received was: '%c'\n", chr);
    //    }

    char operator;
    double operand1, operand2, result;
    while (1) {
        printf("\nEnter a mathematical operation to perform (+,-,*,/,a,m,f,c,t,r): ");
        scanf(" %c", &operator);
        if (operator == 'q') {
            printf("\n");
            return;
        } else if (operator == '+') // operation for add
        {
            printf("\nEnter the first operand: ");
            scanf(" %lf", &operand1);

            printf("Enter the second operand: ");
            scanf(" %lf", &operand2);

            result = Add(operand1, operand2);
            printf("Result of (%lf + %lf): %lf\n", operand1, operand2, result);
            //            printf("You input (%lf + %lf)\n", operand1, operand2);
        } else if (operator == '-') // operation for subtract
        {
            printf("\nEnter the first operand: ");
            scanf(" %lf", &operand1);

            printf("Enter the second operand: ");
            scanf(" %lf", &operand2);

            result = Subtract(operand1, operand2);
            printf("Result of (%lf - %lf): %lf\n", operand1, operand2, result);
            //            printf("You input (%lf - %lf)\n", operand1, operand2);
        } else if (operator == '*') // operation for multiply
        {
            printf("\nEnter the first operand: ");
            scanf(" %lf", &operand1);

            printf("Enter the second operand: ");
            scanf(" %lf", &operand2);

            result = Multiply(operand1, operand2);
            printf("Result of (%lf * %lf): %lf\n", operand1, operand2, result);
            //            printf("You input (%lf * %lf)\n", operand1, operand2);
        } else if (operator == '/') // operation for divide
        {
            printf("\nEnter the first operand: ");
            scanf(" %lf", &operand1);

            printf("Enter the second operand: ");
            scanf(" %lf", &operand2);

            result = Divide(operand1, operand2);
            if (result == HUGE_VAL) {
                printf("Divide by zero error!\n");
            } else {
                printf("Result of (%lf / %lf): %lf\n", operand1, operand2, result);
                //                printf("You input (%lf / %lf)\n", operand1, operand2);
            }
        } else if (operator == 'a') // operation for absolute value
        {
            printf("\nEnter an operand: ");
            scanf(" %lf", &operand1);

            //            // Test cases for AbsoluteValue()
            //            printf("%lf\n", AbsoluteValue(-3)); // result should be 3
            //            printf("%lf\n", AbsoluteValue(-8.63)); // result should be 8.63
            //            printf("%lf\n", AbsoluteValue(0)); // result should be 0
            //            printf("%lf\n", AbsoluteValue(13.67)); // result should be 13.67

            result = AbsoluteValue(operand1);
            printf("Result of |%lf|: %lf\n", operand1, result);
            //            printf("You input |%lf|\n", operand1);
        } else if (operator == 'm') // operation for average
        {
            printf("\nEnter the first operand: ");
            scanf(" %lf", &operand1);

            printf("Enter the second operand: ");
            scanf(" %lf", &operand2);

            //            // Test cases for Average()
            //            printf("%lf\n", Average(55.5, 0)); // result should be 27.75
            //            printf("%lf\n", Average(0.00, -10)); // result should be -5
            //            printf("%lf\n", Average(-36.49, 36.49)); // result should be 0.0

            result = Average(operand1, operand2);
            printf("Result of (%lf + %lf) / 2: %lf\n", operand1, operand2, result);
            //            printf("You input (%lf + %lf) / 2\n", operand1, operand2);   
        } else if (operator == 'f') // operation for Celsius to Fahrenheit
        {
            printf("\nEnter an operand in Celsius: ");
            scanf(" %lf", &operand1);

            //            // Test cases for CelsiusToFahrenheit()
            //            printf("%lf\n", CelsiusToFahrenheit(32)); // result should be 89.599995
            //            printf("%lf\n", CelsiusToFahrenheit(-27)); // result should be -16.599998
            //            printf("%lf\n", CelsiusToFahrenheit(0)); // result should be 32

            result = CelsiusToFahrenheit(operand1);
            printf("Result of (%lf deg -> F): %lf\n", operand1, result);
            //            printf("You input %lf\n", operand1);
        } else if (operator == 'c') // operation for Fahrenheit to Celsius
        {
            printf("\nEnter an operand in Fahrenheit: ");
            scanf(" %lf", &operand1);

            //            // Test cases for FahrenheitToCelsius()
            //            printf("%lf\n", FahrenheitToCelsius(98)); // result should be 36.666668
            //            printf("%lf\n", FahrenheitToCelsius(-12)); // result should be -24.444445
            //            printf("%lf\n", FahrenheitToCelsius(0)); // result should be -17.777779 

            result = FahrenheitToCelsius(operand1);
            printf("Result of (%lf deg -> C): %lf\n", operand1, result);
            //            printf("You input %lf\n", operand1);
        } else if (operator == 't') // operation for tangent
        {
            printf("\nEnter an operand: ");
            scanf(" %lf", &operand1);

            //                        // Test cases for Tangent()
            //                        printf("%lf\n", Tangent(57)); // result should be 1.5399
            //                        printf("%lf\n", Tangent(1.5)); // result should be 0.0262
            //                        printf("%lf\n", Tangent(-33)); // result should be -0.6494 
            //                        printf("%lf\n", Tangent(0)); // result should be 0

            result = Tangent(operand1);
            printf("Result of tan(%lf): %lf\n", operand1, result);
            //            printf("You input %lf\n", operand1);
        } else if (operator == 'r') // operation for round
        {
            printf("\nEnter an operand: ");
            scanf(" %lf", &operand1);

            //            // Test cases for Round()
            //            printf("%lf\n", Round(57.68)); // result should be 58
            //            printf("%lf\n", Round(0.579)); // result should be 1
            //            printf("%lf\n", Round(-33.235)); // result should be -33 
            //            printf("%lf\n", Round(0)); // result should be 0

            result = Round(operand1);
            printf("Result of round(%lf): %lf\n", operand1, result);
            //            printf("You input %lf\n", operand1);
        } else {
            printf("\nError, not a valid operator.\n");
        }
    }

    BOARD_End();
    while (1);
}

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2) {
    return operand1 + operand2;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2) {
    return operand1 - operand2;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2) {
    return operand1 * operand2;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2) {
    if (operand2 == 0)
    {
        return HUGE_VAL;
    }
    else
    {
        return operand1 / operand2;
    }
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand) {
    if ((operand + operand) >= 0) {
        return operand;
    } else {
        return (-1 * operand);
    }
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand) {
    return (5.0 / 9.0) * (operand - 32.0);
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand) {
    return operand * (9.0 / 5.0) + 32.0;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2) {
    return (operand1 + operand2) / 2.0;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand) {
    return tan(DegToRad(operand));
}

/*********************************************************************************
 * Helper function that converts degrees to radians for the Tangent function.
 ********************************************************************************/
double DegToRad(double operand) {
    return operand * (M_PI / 180.0);
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand) {
    int round;
    if (operand < 0) {
        double pos = AbsoluteValue(operand);
        round = (int) pos;
        if ((pos - round) >= 0.5) {
            return (-1 * round) - 1;
        } else {
            return (-1 * round);
        }
    } else {
        round = (int) operand;
        if ((operand - round) >= 0.5) {
            return (1 + round);
        } else {
            return round;
        }
    }
}


