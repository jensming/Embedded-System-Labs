// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"


// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
static uint8_t buttonEvents;

// **** Declare function prototypes ****


int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 2; // 1:64 prescaler
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
    printf("Welcome to 1725314's lab6 part4 (ButtonsTest).  Compiled on %s %s.\n", __TIME__, __DATE__);
    printf("Please press some buttons!\n");
    
    // initialize both Oled and pins to use buttons
    ButtonsInit();
    OledInit();
    
    char strOled[35];
    
    while (1) {
        // dashes indicate that button event has not been detected
        char *btn1 = "----", *btn2 = "----", *btn3 = "----", *btn4 = "----";

        // if button event detected, check which one(s)
        if (buttonEvents != BUTTON_EVENT_NONE) {
            // checks for btn1 button events
            if (buttonEvents & BUTTON_EVENT_1DOWN) {
                btn1 = "DOWN";
            } else if (buttonEvents & BUTTON_EVENT_1UP) {
                btn1 = " UP ";
            }
            // checks for btn2 button events
            if (buttonEvents & BUTTON_EVENT_2DOWN) {
                btn2 = "DOWN";
            } else if (buttonEvents & BUTTON_EVENT_2UP) {
                btn2 = " UP ";
            }
            // checks for btn3 button events
            if (buttonEvents & BUTTON_EVENT_3DOWN) {
                btn3 = "DOWN";
            } else if (buttonEvents & BUTTON_EVENT_3UP) {
                btn3 = " UP ";
            }
            // checks for btn4 button events
            if (buttonEvents & BUTTON_EVENT_4DOWN) {
                btn4 = "DOWN";
            } else if (buttonEvents & BUTTON_EVENT_4UP) {
                btn4 = " UP ";
            }

            // display events detected
            sprintf(strOled, "EVENT:  4:%s 3:%s        2:%s 1:%s\n", btn4, btn3, btn2, btn1);
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(strOled);
            OledUpdate();

            // reset to no button event
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