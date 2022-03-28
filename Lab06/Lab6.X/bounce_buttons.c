// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
#include "Leds_Lab06.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
enum LEDs {
    LED1 = 0x01,
    LED2 = 0x02,
    LED3 = 0x04,
    LED4 = 0x08,
    LED5 = 0x10,
    LED6 = 0x20,
    LED7 = 0x40,
    LED8 = 0x80
};

// **** Define global, module-level, or external variables here ****
static uint8_t buttonEvents;

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
    printf("Welcome to 1725314's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);
    
    // initialize pins of buttons and LEDs
    ButtonsInit();
    LEDS_INIT();
    
    // values to hold state of LEDs to toggle and the state of switches respectfully
    char toggle = 0x00;
    uint8_t switchStates;

    while (1) {
        if (buttonEvents != BUTTON_EVENT_NONE) {
            // get state of LEDS and switches
            toggle = LEDS_GET();
            switchStates = SWITCH_STATES();

            // toggle LED1 and LED2 for both up or down button event
            if ((switchStates & SWITCH_STATE_SW1) && (buttonEvents & BUTTON_EVENT_1UP)) {
                toggle ^= LED1 ^ LED2;
            } else if (!(switchStates & SWITCH_STATE_SW1) && (buttonEvents & BUTTON_EVENT_1DOWN)) {
                toggle ^= LED1 ^ LED2;
            }
            // toggle LED3 and LED4 for both up or down button event
            if ((switchStates & SWITCH_STATE_SW2) && (buttonEvents & BUTTON_EVENT_2UP)) {
                toggle ^= LED3 ^ LED4;
            } else if (!(switchStates & SWITCH_STATE_SW2) && (buttonEvents & BUTTON_EVENT_2DOWN)) {
                toggle ^= LED3 ^ LED4;
            }
            // toggle LED5 and LED6 for both up or down button event
            if ((switchStates & SWITCH_STATE_SW3) && (buttonEvents & BUTTON_EVENT_3UP)) {
                toggle ^= LED5 ^ LED6;
            } else if (!(switchStates & SWITCH_STATE_SW3) && (buttonEvents & BUTTON_EVENT_3DOWN)) {
                toggle ^= LED5 ^ LED6;
            }
            // toggle LED7 and LED8 for both up or down button event
            if ((switchStates & SWITCH_STATE_SW4) && (buttonEvents & BUTTON_EVENT_4UP)) {
                toggle ^= LED7 ^ LED8;
            } else if (!(switchStates & SWITCH_STATE_SW4) && (buttonEvents & BUTTON_EVENT_4DOWN)) {
                toggle ^= LED7 ^ LED8;
            }

            // set LEDs accordingly
            LEDS_SET(toggle);

            // clear button event flag
            buttonEvents = BUTTON_EVENT_NONE;
        }
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    // checks for button events
    buttonEvents = ButtonsCheckEvents();
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}