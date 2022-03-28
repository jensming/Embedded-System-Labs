// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"


// User libraries
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    BOARD_Init();
    // Declare Variables
    float fahr, celsius, kelvin;
    int lower, upper, step;

    // Initialize Variables
    lower = 0; // lower limit of temperature
    upper = 300; // upper limit
    step = 20; // step size
    fahr = lower;
    kelvin = lower;

    // Print out table Fahrenheit-to-Celsius
    printf("    F     C\n");
    while (fahr <= upper) {
        celsius = (5.0 / 9.0) * (fahr - 32.0);
        printf("%7.1f %04.f\n", (double) fahr, (double) celsius);
        fahr = fahr + step;
    }

    printf("\n");

    // Print out table Kelvin-to-Fahrenheit
    printf("     K     F\n");
    while (kelvin <= upper) {
        fahr = (9.0 / 5.0) * (kelvin - 273.15) + 32;
        printf("%03.3f %5.6f\n", (double) kelvin, (double) fahr);
        kelvin = kelvin + step;
    }

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}
