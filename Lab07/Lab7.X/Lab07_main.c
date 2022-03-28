// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CSE13E Support Library
#include "BOARD.h"
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>


// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)


// **** Set any local typedefs here ****
typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, ALERT
} OvenState;

typedef struct {
    OvenState state;
    // add more members to this struct
    uint16_t temperature; // temperature setting of oven
    uint16_t startTime; // total time for cooking
    uint16_t timeRemaining; // time left to cook which can be edited
    uint16_t buttonPressTime; // amount of time a button was held down
    uint8_t cookMode; // determines what cooking mode the oven is in
} OvenData;

typedef enum {
    BAKE, TOAST, BROIL // cooking modes of the oven
} CookingState;

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****
static OvenData ovenData;
static uint8_t TIMER_TICK; // tracks timer events
static uint8_t buttonEvent; // tracks button events
static uint8_t adcEvent; // tracks ADC events
static uint16_t adcValue; // top 8 bits of ADC read used for adjusting cooking temp
static uint16_t freeRunningTime; // global timer for this program
static uint16_t elapsedTime; // time elapsed when BTN3 is down and then up
static uint16_t freeTimeMark; // holds the time before oven enters COOKING state
static uint8_t changingTemp; // is true when selector is on temp
static uint16_t ledInterval; // total time for LED intervals
static uint16_t ledIntervalRemainder; // remainder of total time for LED intervals
static uint16_t cookingTimer; // tracks number of timer events for manipulating oven time
static uint8_t invertOLED; // inverts oven display if true while in ALERT state
static char currentLEDs; // current pattern of LEDs while the oven is in COOKING mode
static char oledPrint[80]; // full display of oven w/ cooking mode, time, and temp
static char topOn[6], topOff[6], bottomOn[6], bottomOff[6]; // oven display components
#define DEFAULT_TEMP 350 // initial temp for oven in BAKE mode
#define LONG_PRESS 5 // button press that lasts for approximately a second based on the 5Hz ISR timer
#define LEDS_ON 0xFF // turns on all LEDs
#define LEDS_OFF 0x00 // turns off all LEDs

// **** Put any helper functions here ****


/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData){
    // update OLED here
    // store special characters used to display oven
    sprintf(topOn, "%s%s%s%s%s", OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON);
    sprintf(topOff, "%s%s%s%s%s", OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF);
    sprintf(bottomOn, "%s%s%s%s%s", OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON);
    sprintf(bottomOff, "%s%s%s%s%s", OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF);
    
    // switch statements below determine what is printed depending on the 
    // current cooking mode and state of the oven
    switch(ovenData.cookMode) {
        case BAKE:
            // making time/temp changes when in SELECTOR_CHANGE_PENDING state 
            // or not in either the COOKING or RESET_PENDING state
            if (!((ovenData.state == COOKING) || (ovenData.state == RESET_PENDING)) ||
                    (ovenData.state == SELECTOR_CHANGE_PENDING)) {
                if (changingTemp) {
                    sprintf(oledPrint, "|%s|  Mode: Bake\n|     |  Time: %d:%02d\n|-----| >Temp: %d%sF\n"
                            "|%s|", topOff, (ovenData.startTime) / 60, (ovenData.startTime) % 60,
                            ovenData.temperature, DEGREE_SYMBOL, bottomOff);
                } else {
                    sprintf(oledPrint, "|%s|  Mode: Bake\n|     | >Time: %d:%02d\n|-----|  Temp: %d%sF\n"
                            "|%s|", topOff, (ovenData.startTime) / 60, (ovenData.startTime) % 60,
                            ovenData.temperature, DEGREE_SYMBOL, bottomOff);
                }
            }                
            // oven is baking something decadent
            else {
                sprintf(oledPrint, "|%s|  Mode: Bake\n|     |  Time: %d:%02d\n|-----|  Temp: %d%sF\n"
                        "|%s|", topOn, (ovenData.timeRemaining) / 60, (ovenData.timeRemaining) % 60,
                        ovenData.temperature, DEGREE_SYMBOL, bottomOn);
            }
            break;

        case TOAST:
            // making time change when not in either the COOKING or RESET_PENDING state
            if (!((ovenData.state == COOKING) || (ovenData.state == RESET_PENDING))) {
                sprintf(oledPrint, "|%s|  Mode: Toast\n|     |  Time: %d:%02d\n|-----|\n"
                        "|%s|", topOff, (ovenData.startTime) / 60, (ovenData.startTime) % 60, bottomOff);
            }                
            // oven is toasting something scrumptious
            else {
                sprintf(oledPrint, "|%s|  Mode: Toast\n|     |  Time: %d:%02d\n|-----|\n"
                        "|%s|", topOff, (ovenData.timeRemaining) / 60, (ovenData.timeRemaining) % 60, bottomOn);
            }
            break;

        case BROIL:
            // making time change when not in either the COOKING or RESET_PENDING state
            if (!((ovenData.state == COOKING) || (ovenData.state == RESET_PENDING))) {
                sprintf(oledPrint, "|%s|  Mode: Broil\n|     |  Time: %d:%02d\n|-----|  Temp: 500%sF\n"
                        "|%s|", topOff, (ovenData.startTime) / 60, (ovenData.startTime) % 60,
                        DEGREE_SYMBOL, bottomOff);
            }                
            // oven is broiling something delicious
            else {
                sprintf(oledPrint, "|%s|  Mode: Broil\n|     |  Time: %d:%02d\n|-----|  Temp: 500%sF\n"
                        "|%s|", topOn, (ovenData.timeRemaining) / 60, (ovenData.timeRemaining) % 60,
                        DEGREE_SYMBOL, bottomOff);
            }
            break;
    }

    // display the oven details on OLED depending on its cooking mode and state
    OledClear(OLED_COLOR_BLACK);
    OledDrawString(oledPrint);
    if (ovenData.state == ALERT) {
        if (invertOLED) {
            OledSetDisplayInverted();
        } else {
            OledSetDisplayNormal();
        }
    } else {
        OledSetDisplayNormal();
    }
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void) {
    // write your SM logic here.
    switch (ovenData.state) {
        case SETUP:
            if (adcEvent) {
                // isolate top 8 bits by bitwise shifting right twice and then 
                // bit masking those 8 bits
                adcValue = (AdcRead() >> 2) & 0x00FF;

                // adjusting oven temp with pot if in BAKE mode and changingTemp is true
                if ((ovenData.cookMode == BAKE) && changingTemp) {
                    ovenData.temperature = adcValue + 300;
                }                    
                // adjusting oven time with pot
                else {
                    ovenData.startTime = adcValue + 1;
                    ovenData.timeRemaining = ovenData.startTime;
                }

                // update oven OLED
                updateOvenOLED(ovenData);
            }

            if (buttonEvent & BUTTON_EVENT_3DOWN) {
                // measure the amount of time the button was pressed and then
                // change the oven's state to SELECTOR_CHANGE_PENDING
                ovenData.buttonPressTime = freeRunningTime;
                ovenData.state = SELECTOR_CHANGE_PENDING;
            }

            if (buttonEvent & BUTTON_EVENT_4DOWN) {
                // save free-running time and update OLED with new oven state
                freeTimeMark = freeRunningTime;
                ovenData.state = COOKING;
                updateOvenOLED(ovenData);
                // turn on LEDs, calculate 1/8th of the oven time to toggle LEDs
                // when cooking, and initialize the cooking timer for determining
                // when the oven time decrements in the COOKING state
                LEDS_SET(LEDS_ON);
                ledInterval = (ovenData.startTime * 5) / 8;
                ledIntervalRemainder = (ovenData.startTime * 5) % 8;
                cookingTimer = 0x00;
            }
            break;

        case SELECTOR_CHANGE_PENDING:
            if (buttonEvent & BUTTON_EVENT_3UP) {
                // measure elapsed time of button press and determine if the selector
                // is on the temp or time setting
                elapsedTime = freeRunningTime - ovenData.buttonPressTime;
                if ((elapsedTime >= LONG_PRESS) && (ovenData.cookMode == BAKE)) {
                    if (changingTemp) {
                        changingTemp = FALSE;
                    } else {
                        changingTemp = TRUE;
                    }

                    // update state of oven and oven OLED
                    ovenData.state = SETUP;
                    updateOvenOLED(ovenData);
                } else {
                    // if button press was short, change cooking mode of oven
                    // the order is as follows: BAKE, TOAST, BROIL
                    if (ovenData.cookMode == BAKE) {
                        ovenData.cookMode = TOAST;
                    } else if (ovenData.cookMode == TOAST) {
                        ovenData.cookMode = BROIL;
                    } else {
                        ovenData.cookMode = BAKE;
                    }

                    // update state of oven and oven OLED
                    ovenData.state = SETUP;
                    updateOvenOLED(ovenData);
                }
            }
            break;

        case COOKING:
            if (TIMER_TICK) {
                // update cooking timer at ever timer event
                cookingTimer++;
                if ((cookingTimer == ledInterval) && (ledIntervalRemainder >= 0)) {
                    // check to see if an LED needs to turn off at every timer event,
                    // update variables tracking time of LED intervals
                    currentLEDs = LEDS_GET();
                    LEDS_SET(currentLEDs << 1);
                    ledIntervalRemainder--;
                    cookingTimer = 0x00;
                }

                if (((freeRunningTime - freeTimeMark) % 5) == 0) {
                    // if a second has passed on the timer, decrement the oven time
                    // by a second as well
                    ovenData.timeRemaining--;
                    updateOvenOLED(ovenData);
                }

                if (ovenData.timeRemaining == 0) {
                    // when the oven time is done, reset the oven time to the initial 
                    // configured time, turn off all LEDs, and then transition to
                    // flashing ALERT oven state
                    ovenData.timeRemaining = ovenData.startTime;
                    ovenData.state = ALERT;
                    LEDS_SET(LEDS_OFF);
                    updateOvenOLED(ovenData);
                }
            }

            if (buttonEvent & BUTTON_EVENT_4DOWN) {
                // save free-running time and set oven to RESET_PENDING state
                // to reset the oven settings on display
                ovenData.buttonPressTime = freeRunningTime;
                ovenData.state = RESET_PENDING;
            }
            break;

        case RESET_PENDING:
            // oven is still cooking and continues with COOKING state logic 
            // until the elapsed time that BTN4 is held down surpasses a second
            if (TIMER_TICK) {
                // update cooking timer at ever timer event
                cookingTimer++;
                if ((cookingTimer == ledInterval) && (ledIntervalRemainder >= 0)) {
                    // check to see if an LED needs to turn off at every timer event,
                    // update variables tracking time of LED intervals
                    currentLEDs = LEDS_GET();
                    LEDS_SET(currentLEDs << 1);
                    ledIntervalRemainder--;
                    cookingTimer = 0x00;
                }

                if (((freeRunningTime - freeTimeMark) % 5) == 0) {
                    // if a second has passed on the timer, decrement the oven time
                    // by a second as well
                    ovenData.timeRemaining--;
                    updateOvenOLED(ovenData);
                }

                elapsedTime = freeRunningTime - ovenData.buttonPressTime;
                if (elapsedTime >= LONG_PRESS) {
                    // when BTN4 is held down for longer than a second,
                    // the oven resets to its initial state before cooking along
                    // with the LEDs, time, and temp if in BAKE mode
                    ovenData.timeRemaining = ovenData.startTime;
                    ovenData.state = SETUP;
                    LEDS_SET(LEDS_OFF);
                    updateOvenOLED(ovenData);
                }
            }

            elapsedTime = freeRunningTime - ovenData.buttonPressTime;
            if ((buttonEvent & BUTTON_EVENT_4UP) && (elapsedTime < LONG_PRESS)) {
                // when BTN4 is released before a second has passed,
                // the oven goes back to the COOKING state to continue cooking
                ovenData.state = COOKING;
            }
            break;

        case ALERT:
            if (TIMER_TICK) {
                // oven display should flash by inverting the OLED each time a timer event occurs
                if (!invertOLED) {
                    invertOLED = TRUE;
                } else {
                    invertOLED = FALSE;
                }
                updateOvenOLED(ovenData);
            }

            if (buttonEvent & BUTTON_EVENT_4UP) {
                // when BTN4 is pressed and released, it should reset the oven
                // display and return to the SETUP state
                ovenData.timeRemaining = ovenData.startTime;
                ovenData.state = SETUP;
                invertOLED = FALSE;
                updateOvenOLED(ovenData);
            }
            break;
    }
}


int main()
{
    BOARD_Init();

     // initialize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">
    
    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
   
    printf("Welcome to 1725314's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    // initialize state machine (and anything else you need to init) here
    OledInit();
    AdcInit();
    ButtonsInit();
    LEDS_INIT();
    
    // initialize ovenData and other variables
    ovenData.state = SETUP;
    ovenData.startTime = 0x01;
    ovenData.timeRemaining = ovenData.startTime;
    ovenData.buttonPressTime = 0x00;
    ovenData.temperature = DEFAULT_TEMP;
    ovenData.cookMode = BAKE;
    changingTemp = FALSE;
    freeRunningTime = 0x00;
    invertOLED = FALSE;
    
    // update OLED with initial oven state
    updateOvenOLED(ovenData);

    while (1){
        // Add main loop code here:
        // check for events
        // on event, run runOvenSM()
        // clear event flags
        if (TIMER_TICK || adcEvent || (buttonEvent != BUTTON_EVENT_NONE)) {
            runOvenSM();
            TIMER_TICK = FALSE;
            adcEvent = FALSE;
            buttonEvent = BUTTON_EVENT_NONE;
        }
    };
}


/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    // add event-checking code here
    TIMER_TICK = TRUE; // generate timer event
    freeRunningTime++; // update free-running timer
}


/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    // add event-checking code here
    buttonEvent = ButtonsCheckEvents(); // check button events
    adcEvent = AdcChanged(); // check ADC events
}