/* 
 * File:   Leds_Lab06.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on February 8, 2022, 11:18 AM
 */

// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab06.h"

// **** Declared function prototypes ****
void LEDS_INIT(void);
void LEDS_SET(char newPattern);
char LEDS_GET(void);

/********************************************************************************
 * Defined LEDS_INIT function.
 ********************************************************************************/
void LEDS_INIT(void)
{
    TRISE = 0x00; // sets each LED pin as output
    LATE = 0x00; // sets each LED pin's output value to zero
}

/********************************************************************************
 * Defined LEDS_SET function.
 ********************************************************************************/
void LEDS_SET(char newPattern)
{
    LATE = newPattern; // sets the LEDs to newPattern
}

/********************************************************************************
 * Defined LEDS_GET function.
 ********************************************************************************/
char LEDS_GET(void)
{
    return LATE; // return current state of LED bar as on or off
}