/* 
 * File:   Buttons.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on February 8, 2022, 11:32 AM
 */

// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// **** Defined global, module-level, or external variables ****
static uint8_t debounceTime;
static uint8_t prevButtonStates;

// **** Declared function prototypes ****
void ButtonsInit(void);
uint8_t ButtonsCheckEvents(void);

/********************************************************************************
 * Defined ButtonsInit function.
 ********************************************************************************/
void ButtonsInit(void)
{
    // enable D5/6/7 and F1 pins as inputs for the microprocessor
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
    
    // initialize module-level variables to zero to check for initial button presses
    debounceTime = 0x00;
    prevButtonStates = 0x00;
}

/********************************************************************************
 * Defined ButtonsCheckEvents function.
 ********************************************************************************/
uint8_t ButtonsCheckEvents(void)
{
    // initialize button events to none at start and get state of buttons
    uint8_t buttonEvents = BUTTON_EVENT_NONE;
    uint8_t buttonStates = BUTTON_STATES();
    
    // won't check for button events until after debounce period ends
    if (debounceTime > 0x00) {
        debounceTime--;
        return buttonEvents;
    }
    // no button event if previous and current button states are the same
    else if (buttonStates == prevButtonStates) {
        return BUTTON_EVENT_NONE;
    }
    
    // checks for all possible buttons events that could occur for BTN1
    if ((buttonStates & BUTTON_STATE_1) && !(prevButtonStates & BUTTON_STATE_1)) {
        buttonEvents |= BUTTON_EVENT_1DOWN;
    } else if (!(buttonStates & BUTTON_STATE_1) && (prevButtonStates & BUTTON_STATE_1)) {
        buttonEvents |= BUTTON_EVENT_1UP;
    }
    // checks for all possible buttons events that could occur for BTN2
    if ((buttonStates & BUTTON_STATE_2) && !(prevButtonStates & BUTTON_STATE_2)) {
        buttonEvents |= BUTTON_EVENT_2DOWN;
    } else if (!(buttonStates & BUTTON_STATE_2) && (prevButtonStates & BUTTON_STATE_2)) {
        buttonEvents |= BUTTON_EVENT_2UP;
    }
    // checks for all possible buttons events that could occur for BTN3
    if ((buttonStates & BUTTON_STATE_3) && !(prevButtonStates & BUTTON_STATE_3)) {
        buttonEvents |= BUTTON_EVENT_3DOWN;
    } else if (!(buttonStates & BUTTON_STATE_3) && (prevButtonStates & BUTTON_STATE_3)) {
        buttonEvents |= BUTTON_EVENT_3UP;
    }
    // checks for all possible buttons events that could occur for BTN4
    if ((buttonStates & BUTTON_STATE_4) && !(prevButtonStates & BUTTON_STATE_4)) {
        buttonEvents |= BUTTON_EVENT_4DOWN;
    } else if (!(buttonStates & BUTTON_STATE_4) && (prevButtonStates & BUTTON_STATE_4)) {
        buttonEvents |= BUTTON_EVENT_4UP;
    }
    
    // reset debounce period, current buttons states are now previous ones
    debounceTime = BUTTONS_DEBOUNCE_PERIOD;
    prevButtonStates = buttonStates;
    
    return buttonEvents;
}