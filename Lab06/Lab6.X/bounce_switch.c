
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
					
//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"


// **** Declare any datatypes here ****
struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
static struct Timer Timer1;
enum Directions { 
    MOVING_LEFT,
    MOVING_RIGHT
};
static uint8_t switchesState; // holds state of switches
static uint8_t switchesCount; // counts number of switches up
static int switchesScale; // scales the speed of bouncing LEDs
//#define TIME 76

// **** Declare function prototypes ****


int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to 1725314's lab6 part2 (bounce_switch).  Compiled on %s %s.\n",__TIME__,__DATE__);

    // initialize LEDs
    LEDS_INIT();
    
    // initialize Timer1 and while-loop variables
    Timer1.event = FALSE;
//    Timer1.timeRemaining = TIME;
    char leftEdge = 0x80;
    char rightEdge = 0x01;
    char currentLED = rightEdge;
    uint8_t state;
							 
	while(1){
        //poll timer events and react if any occur
        if (Timer1.event == TRUE) {
            // set initial LED to currentLED
            LEDS_SET(currentLED);

            // if LED is at leftEdge, reverse direction state
            if (currentLED == leftEdge) {
                state = MOVING_RIGHT;
            }                
            // if LED is at rightEdge, reverse direction state
            else if (currentLED == rightEdge) {
                state = MOVING_LEFT;
            }

            // if LED is not an edge case and is moving left, bitwise left shift currentLED by 1
            if (state == MOVING_LEFT) {
                currentLED <<= 1;
            }                
            // if LED is not an edge case and is moving right, bitwise right shift currentLED by 1
            else {
                // accounts for padding at leftEdge case when bitwise right shifting currentLED by 1
                if (currentLED == leftEdge) {
                    currentLED = (currentLED >> 1) ^ 0x80;
                }
                else {
                    currentLED >>= 1;
                }
            }
            //            else
            //            {
            //                printf("error with state!\n");
            //            }
            
            // clear Timer1 event flag
            Timer1.event = FALSE;
        }
    }			


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}


/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    //update Timer1 and initialize scaling
    Timer1.timeRemaining--;
    switchesScale = 2;
    
    // when timeRemaining is zero, time to turn on next LED
    if (Timer1.timeRemaining <= 0) {
        // generate Timer1 event, get state of switches, and initialize count of switches that are up
        Timer1.event = TRUE;
        switchesState = SWITCH_STATES();
        switchesCount = 0x00;

        // checks if switch 1 is up
        if (switchesState & SWITCH_STATE_SW1) {
            switchesCount |= SWITCH_STATE_SW1;
        }
        // checks if switch 2 is up
        if (switchesState & SWITCH_STATE_SW2) {
            switchesCount |= SWITCH_STATE_SW2;
        }
        // checks if switch 3 is up
        if (switchesState & SWITCH_STATE_SW3) {
            switchesCount |= SWITCH_STATE_SW3;
        }
        // checks if switch 4 is up
        if (switchesState & SWITCH_STATE_SW4) {
            switchesCount |= SWITCH_STATE_SW4;
        }
        
        // set timeRemaining to the scaled speed based on the number of switches that are up
        switchesCount *= switchesScale;
        Timer1.timeRemaining = switchesCount;
    }
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/									
	 
}