/* 
 * File:   AgentTest.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on March 10, 2022, 9:47 PM
 */

// Standard libraries
#include <stdio.h>

// User libraries
#include "Agent.h"
#include "BOARD.h"
#include "Uart1.h"


// **** Define any module-level, global, or external variables here ****
#define AIGS 1 // used to test for AgentInit(), AgentGetState(), and AgentSetState()
#define AR 1 // used to test for AgentRun()
static int testCount; // tracks number of passed tests
BB_Event event; // holds BattleBoats event data

int main() {
    BOARD_Init();

    printf("Testing Agent functions:\n\n");

#if AIGS
    printf("AgentInit(), AgentGetState(), and AgentSetState()\n");

    // testing to see if agent was initialized to correct first state
    AgentInit();
    if (AgentGetState() == AGENT_STATE_START) {
        printf("    AgentInit() is working!\n");
    } else {
        printf("    AgentInit() failed.\n");
    }

    // testing for correct agent state transitions
    testCount = 0;
    AgentSetState(AGENT_STATE_CHALLENGING);
    if (AgentGetState() == AGENT_STATE_CHALLENGING) {
        testCount++;
    }
    AgentSetState(AGENT_STATE_ACCEPTING);
    if (AgentGetState() == AGENT_STATE_ACCEPTING) {
        testCount++;
    }
    AgentSetState(AGENT_STATE_ATTACKING);
    if (AgentGetState() == AGENT_STATE_ATTACKING) {
        testCount++;
    }
    AgentSetState(AGENT_STATE_DEFENDING);
    if (AgentGetState() == AGENT_STATE_DEFENDING) {
        testCount++;
    }
    AgentSetState(AGENT_STATE_WAITING_TO_SEND);
    if (AgentGetState() == AGENT_STATE_WAITING_TO_SEND) {
        testCount++;
    }
    AgentSetState(AGENT_STATE_END_SCREEN);
    if (AgentGetState() == AGENT_STATE_END_SCREEN) {
        testCount++;
    }
    if (testCount == 6) {
        printf("    AgentGetState() and AgentSetState() are working!\n\n");
    } else {
        printf("    AgentGetState() and or AgentSetState() failed.\n\n");
    }
#endif

#if AR
    printf("AgentRun()\n");

    event.type = BB_EVENT_START_BUTTON;
    event.param0 = 0;
    event.param1 = 0;
    event.param2 = 0;
    AgentInit();
    AgentRun(event);

    // testing for correct state transitions within the state machine
    testCount = 0;
    if (AgentGetState() == AGENT_STATE_CHALLENGING) {
        testCount++;
    }
    event.type = BB_EVENT_ACC_RECEIVED;
    AgentRun(event);
    if (AgentGetState() == AGENT_STATE_WAITING_TO_SEND) {
        testCount++;
        event.type = BB_EVENT_MESSAGE_SENT;
        AgentRun(event);
        if (AgentGetState() == AGENT_STATE_ATTACKING) {
            testCount++;
        }
        event.type = BB_EVENT_RES_RECEIVED;
        AgentRun(event);
        if (AgentGetState() == AGENT_STATE_DEFENDING) {
            testCount++;
        }
        event.type = BB_EVENT_SHO_RECEIVED;
        AgentRun(event);
        if (AgentGetState() == AGENT_STATE_WAITING_TO_SEND) {
            testCount++;
        }
    } else if (AgentGetState() == AGENT_STATE_DEFENDING) {
        testCount++;
        event.type = BB_EVENT_SHO_RECEIVED;
        AgentRun(event);
        if (AgentGetState() == AGENT_STATE_WAITING_TO_SEND) {
            testCount++;
        }
        event.type = BB_EVENT_MESSAGE_SENT;
        AgentRun(event);
        if (AgentGetState() == AGENT_STATE_ATTACKING) {
            testCount++;
        }
        event.type = BB_EVENT_RES_RECEIVED;
        AgentRun(event);
        if (AgentGetState() == AGENT_STATE_DEFENDING) {
            testCount++;
        }
    }
    if (testCount == 5) {
        printf("    AgentRun() works!\n");
    } else {
        printf("    AgentRun() failed.\n");
    }

#endif

    BOARD_End();
    while (1);
}