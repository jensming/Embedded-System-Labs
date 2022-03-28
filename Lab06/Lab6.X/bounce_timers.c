// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

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
static struct Timer TimerA;
static struct Timer TimerB;
static struct Timer TimerC;
#define TIME_A 76 // lit LED changes 76 times in 2 seconds at base rate of timer interrupt
#define TIME_B 114 // lit LED changes 114 times in 3 seconds at base rate of timer interrupt
#define TIME_C 190 // lit LED changes 190 times in 5 seconds at base rate of timer interrupt

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
    printf("Welcome to 1725314's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);

    // initialize LEDs
    LEDS_INIT();
    
    //initialize Timer variables
    TimerA.event = FALSE;
    TimerA.timeRemaining = TIME_A;
    TimerB.event = FALSE;
    TimerB.timeRemaining = TIME_B;
    TimerC.event = FALSE;
    TimerC.timeRemaining = TIME_C;
    
    // value indicating which LED to toggle
    char toggle = 0x00;
    
    while (1) {
        //poll timer A
        //react to timer A events
        //clear timer A event flag
        if (TimerA.event == TRUE) {
            printf("A");
            TimerA.event = FALSE;
            toggle ^= 0x01;
        }
            //poll timer B
            //react to timer B events
            //clear timer B event flag
        if (TimerB.event == TRUE) {
            printf("B");
            TimerB.event = FALSE;
            toggle ^= 0x02;
        }
            //poll timer C
            //react to timer C events
            //clear timer C event flag
        if (TimerC.event == TRUE) {
            printf("C");
            TimerC.event = FALSE;
            toggle ^= 0x04;
        }
        // output to LED bar
        LEDS_SET(toggle);
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
    
    //update timerA
    //if timerA has counted down,
        //generate timerA event
        //reset timerA
    TimerA.timeRemaining--;
    if (TimerA.timeRemaining == 0) {
        TimerA.event = TRUE;
        TimerA.timeRemaining = TIME_A;
    }

    //update timerB
    //if timerB has counted down,
    //generate timerB event
    //reset timerB
    TimerB.timeRemaining--;
    if (TimerB.timeRemaining == 0) {
        TimerB.event = TRUE;
        TimerB.timeRemaining = TIME_B;
    }

    //update timerC
    //if timerC has counted down,
    //generate timerC event
    //reset timerC
    TimerC.timeRemaining--;
    if (TimerC.timeRemaining == 0) {
        TimerC.event = TRUE;
        TimerC.timeRemaining = TIME_C;
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}