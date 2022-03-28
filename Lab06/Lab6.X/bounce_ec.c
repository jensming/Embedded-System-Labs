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
#include "Oled.h"
#include "OledDriver.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};
struct AdcResult {
    uint8_t event;
    uint16_t voltage;
};

// **** Define global, module-level, or external variables here ****
static struct Timer Timer1;
static struct AdcResult ADC1;
static uint8_t buttonEvents;
static uint16_t average; // average of ADC readings
enum Directions { 
    MOVING_LEFT,
    MOVING_RIGHT
};
#define SIZE 5 // size of window for triggering ADC events
#define MAX 1023.0 // max voltage of raw ADC reading
#define MIN 0 // min voltage of raw ADC reading
#define SCALE 5 // scales the spead of bouncing LEDs

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


    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to 1725314's lab6 part6 (Extra Credit).  Compiled on %s %s.\n",__TIME__,__DATE__);

    // initialize OLED, button pins, and LEDs
    OledInit();
    ButtonsInit();
    LEDS_INIT();
    
    // initialize Timer1, ADC1, and while-loop variables
    Timer1.event = FALSE;
    ADC1.event = TRUE;
    char leftEdge = 0x80;
    char rightEdge = 0x01;
    char currentLED = rightEdge;
    uint8_t state;
    uint8_t switchStates;
    int frozen = 0;
    char strOled[30];
    
    while (1)
    {
        // displays raw and percentage ADC voltage readings if event is generated
        if (ADC1.event == TRUE) {
            int percentage = (ADC1.voltage / MAX) * 100;
            sprintf(strOled, "Potentiometer value:\n %d\n %d%%", ADC1.voltage, percentage);

            OledClear(OLED_COLOR_BLACK);
            OledDrawString(strOled);
            OledUpdate();
            ADC1.event = FALSE;
        }
        
        if (buttonEvents != BUTTON_EVENT_NONE) {
            // obtain state of switches
            switchStates = SWITCH_STATES();

            // freeze or unfreeze bouncing LEDs depending on BTN1 event and SW1 state
            if ((switchStates & SWITCH_STATE_SW1) && (buttonEvents & BUTTON_EVENT_1UP)) {
                if (frozen > 0) {
                    frozen = 0;
                } else {
                    frozen = 1;
                }
            } else if (!(switchStates & SWITCH_STATE_SW1) && (buttonEvents & BUTTON_EVENT_1DOWN)) {
                if (frozen > 0) {
                    frozen = 0;
                } else {
                    frozen = 1;
                }
            }
            // freeze or unfreeze bouncing LEDs depending on BTN2 event and SW2 state
            if ((switchStates & SWITCH_STATE_SW2) && (buttonEvents & BUTTON_EVENT_2UP)) {
                if (frozen > 0) {
                    frozen = 0;
                } else {
                    frozen = 1;
                }
            } else if (!(switchStates & SWITCH_STATE_SW2) && (buttonEvents & BUTTON_EVENT_2DOWN)) {
                if (frozen > 0) {
                    frozen = 0;
                } else {
                    frozen = 1;
                }
            }
            // freeze or unfreeze bouncing LEDs depending on BTN3 event and SW3 state
            if ((switchStates & SWITCH_STATE_SW3) && (buttonEvents & BUTTON_EVENT_3UP)) {
                if (frozen > 0) {
                    frozen = 0;
                } else {
                    frozen = 1;
                }
            } else if (!(switchStates & SWITCH_STATE_SW3) && (buttonEvents & BUTTON_EVENT_3DOWN)) {
                if (frozen > 0) {
                    frozen = 0;
                } else {
                    frozen = 1;
                }
            }
            // freeze or unfreeze bouncing LEDs depending on BTN4 event and SW4 state
            if ((switchStates & SWITCH_STATE_SW4) && (buttonEvents & BUTTON_EVENT_4UP)) {
                if (frozen > 0) {
                    frozen = 0;
                } else {
                    frozen = 1;
                }
            } else if (!(switchStates & SWITCH_STATE_SW4) && (buttonEvents & BUTTON_EVENT_4DOWN)) {
                if (frozen > 0) {
                    frozen = 0;
                } else {
                    frozen = 1;
                }
            }

            // clear button event flag
            buttonEvents = BUTTON_EVENT_NONE;
        }
        
        // if there's a timer event and LED isn't frozen, proceed with bouncing LED logic
        if ((Timer1.event == TRUE) && !frozen) {
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
            
            // clear Timer1 event flag
            Timer1.event = FALSE;
        }
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    
    // update Timer1
    Timer1.timeRemaining--;
    // when Timer1 completely counts down, generate Timer1 event and 
    // set timeRemaining according to ADC reading percentage
    if (Timer1.timeRemaining <= 0) {
        Timer1.event = TRUE;
        Timer1.timeRemaining = (100 - ((ADC1.voltage / MAX) * 100)) / SCALE;
    }
    
    // check for button events
    buttonEvents = ButtonsCheckEvents();

}

/**
 * This is the ISR for the ADC1 peripheral.
 */
void __ISR(_ADC_VECTOR, IPL2AUTO) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    // average ADC reading
    average = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7) / 8;
    
    // update voltage measurement if difference between ADC reading and previous average is greater than window
    if (((average - ADC1.voltage) >= SIZE) || ((ADC1.voltage - average) >= SIZE)) {
        ADC1.event = TRUE;
        // check corner cases for when ADC reading is near min and max voltages, else set measurement to average
        if ((average + SIZE) >= MAX) {
            ADC1.voltage = MAX;
        } else if ((average - SIZE) <= MIN) {
            ADC1.voltage = MIN;
        } else {
            ADC1.voltage = average;
        }
    }
}