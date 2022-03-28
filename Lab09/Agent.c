/* 
 * File:   Agent.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on March 9, 2022, 11:54 AM
 */

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// User libraries
#include "Agent.h"
#include "Field.h"
#include "Message.h"
#include "Negotiation.h"
#include "BOARD.h"
#include "FieldOled.h"


// **** Declare any data types here ****
struct Agent {
    AgentState state;
    Field own, opp;
    Message message;
    NegotiationData secret, hash;
    GuessData ownGuess, oppGuess;
};

// **** Define any module-level, global, or external variables here ****
static struct Agent agent;
static int turnCount; // tracks number of turns
static FieldOledTurn turn; // tracks whose turn it is
static NegotiationOutcome coinFlip; // tracks result of coin flip
#define ALL_FIELD_BOATS_SUNK 0b00000000 // endgame return value from FieldGetBoatStates()


// **** Declared function prototypes ****
void AgentInit(void);
Message AgentRun(BB_Event event);
AgentState AgentGetState(void);
void AgentSetState(AgentState newState);
/********************************************************************************
 * Defined AgentInit function.
 ********************************************************************************/
void AgentInit(void) {
    // initialize the state of agent along with turn count and whose turn it is
    // to none
    agent.state = AGENT_STATE_START;
    turnCount = 0;
    turn = FIELD_OLED_TURN_NONE;
}

/********************************************************************************
 * Defined AgentRun function.
 ********************************************************************************/
Message AgentRun(BB_Event event) {
    // set seed for random number generation
    //    srand(time(NULL));

    // determine what BB_Event occurred during the game
    switch (event.type) {
        case BB_EVENT_NO_EVENT:
            // no message, so return none
            agent.message.type = MESSAGE_NONE;
            break;

        case BB_EVENT_START_BUTTON:
            // when in the starting state, generate agent's secret number, hash it, 
            // and set up the fields for the game, then transition into the challenging state
            if (agent.state == AGENT_STATE_START) {
                // hashing secret commitment and set challenge message
                agent.secret = rand() & 0xF0F0;
                agent.message.param0 = NegotiationHash(agent.secret);
                agent.message.type = MESSAGE_CHA;

                // field setup
                FieldInit(&agent.own, &agent.opp);
                FieldAIPlaceAllBoats(&agent.own);

                // set next state to challenging state
                agent.state = AGENT_STATE_CHALLENGING;
            } else {
                // no message if agent isn't in starting state
                agent.message.type = MESSAGE_NONE;
            }
            break;

        case BB_EVENT_RESET_BUTTON:
            // reset the game screen along with agent
            OledClear(OLED_COLOR_BLACK);
            OledDrawString("Press BTN4 to play or wait for challenge.\n");
            OledUpdate();
            AgentInit();

            // set message to none and return it
            agent.message.type = MESSAGE_NONE;
            return agent.message;
            break;

        case BB_EVENT_CHA_RECEIVED:
            // when receiving a challenge, generate agent's secret number and 
            // send it to the opponent, then transition to the accepting state
            if (agent.state == AGENT_STATE_START) {
                // generate secret commitment, store opponent's secret number,
                // and set accept message along with its parameters
                agent.secret = rand() & 0x0F0F;
                agent.hash = event.param0;
                agent.message.param0 = agent.secret;
                agent.message.type = MESSAGE_ACC;

                // set up field
                FieldInit(&agent.own, &agent.opp);
                FieldAIPlaceAllBoats(&agent.own);

                // set next state to accepting state
                agent.state = AGENT_STATE_ACCEPTING;
            } else {
                // no message if agent isn't in starting state
                agent.message.type = MESSAGE_NONE;
            }
            break;

        case BB_EVENT_ACC_RECEIVED:
            // when a challenger, do a coin flip with agent and opponent's secret
            // number to determine who's attacking/defending, then go into state 
            // that handles attack/defense mechanism of game
            if (agent.state == AGENT_STATE_CHALLENGING) {
                // prepare reveal message for agent
                agent.message.type = MESSAGE_REV;
                agent.message.param0 = agent.secret;

                // carry out coin flip (HEADS for agent and TAILS for opponent) 
                // and determine who is attacking/defending
                coinFlip = NegotiateCoinFlip(agent.secret, event.param0);
                if (coinFlip == HEADS) {
                    turn = FIELD_OLED_TURN_MINE;
                    agent.state = AGENT_STATE_WAITING_TO_SEND;
                } else {
                    turn = FIELD_OLED_TURN_THEIRS;
                    agent.state = AGENT_STATE_DEFENDING;
                }
            } else {
                // no message if agent isn't in challenging state
                agent.message.type = MESSAGE_NONE;
            }
            break;

        case BB_EVENT_REV_RECEIVED:
            // when an acceptor, do a coin flip with agent and opponent's secret
            // number to determine who's attacking/defending, then go into state 
            // that handles attack/defense mechanism of game
            if (agent.state == AGENT_STATE_ACCEPTING) {
                // check if the opponent cheated and end the game if detected
                if (NegotiationVerify(event.param0, agent.hash) == FALSE) {
                    agent.message.type = MESSAGE_NONE;
                    agent.state = AGENT_STATE_END_SCREEN;
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Game ended due to cheating message.\n");
                    OledUpdate();
                    return agent.message;
                } else {
                    // carry out coin flip (HEADS for agent and TAILS for opponent) 
                    // and determine who is attacking/defending
                    coinFlip = NegotiateCoinFlip(agent.secret, event.param0);
                    if (coinFlip == HEADS) {
                        // generate a shot guess that attempts to target opponent's boats
                        turn = FIELD_OLED_TURN_MINE;
                        agent.ownGuess = FieldAIDecideGuess(&agent.opp);
                        agent.message.type = MESSAGE_SHO;
                        agent.message.param0 = agent.ownGuess.row;
                        agent.message.param1 = agent.ownGuess.col;
                        agent.state = AGENT_STATE_ATTACKING;
                    } else {
                        // opponent's turn now, agent now defends
                        turn = FIELD_OLED_TURN_THEIRS;
                        agent.message.type = MESSAGE_NONE;
                        agent.state = AGENT_STATE_DEFENDING;
                    }
                }
            } else {
                // no message if agent isn't in accepting state
                agent.message.type = MESSAGE_NONE;
            }
            break;

        case BB_EVENT_SHO_RECEIVED:
            // when defending, register the opponent's attack and update the boats
            // in agent's field accordingly, lost the game if all agent's boats sank
            if (agent.state == AGENT_STATE_DEFENDING) {
                // register the opponent's attack by updating agent's field
                agent.oppGuess.row = event.param0;
                agent.oppGuess.col = event.param1;
                FieldRegisterEnemyAttack(&agent.own, &agent.oppGuess);

                // set result message along with parameters after registering attack
                agent.message.type = MESSAGE_RES;
                agent.message.param0 = agent.oppGuess.row;
                agent.message.param1 = agent.oppGuess.col;
                agent.message.param2 = agent.oppGuess.result;

                // end the game in defeat if all boats sunk in agent's field 
                if (FieldGetBoatStates(&agent.own) == ALL_FIELD_BOATS_SUNK) {
                    agent.message.type = MESSAGE_NONE;
                    agent.state = AGENT_STATE_END_SCREEN;
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Game over, you were defeated. Better luck next time!\n");
                    OledUpdate();
                    return agent.message;
                } else {
                    // agent's turn now while opponent defends
                    turn = FIELD_OLED_TURN_MINE;
                    agent.state = AGENT_STATE_WAITING_TO_SEND;
                }
            } else {
                // no message if agent isn't in defending state
                agent.message.type = MESSAGE_NONE;
            }
            break;

        case BB_EVENT_RES_RECEIVED:
            // when attacking, generate shot guess to attack opponent and update
            // the boats in their field accordingly, won the game if all their boats sank
            if (agent.state == AGENT_STATE_ATTACKING) {
                // register agent's attack by updating opponent's field
                agent.ownGuess.row = event.param0;
                agent.ownGuess.col = event.param1;
                agent.ownGuess.result = event.param2;
                FieldUpdateKnowledge(&agent.opp, &agent.ownGuess);

                // end the game in victory if all boats sunk in opponent's field 
                if (FieldGetBoatStates(&agent.opp) == ALL_FIELD_BOATS_SUNK) {
                    agent.message.type = MESSAGE_NONE;
                    agent.state = AGENT_STATE_END_SCREEN;
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Game over, you won!\n");
                    OledUpdate();
                    return agent.message;
                } else {
                    // opponent's turn now, agent now defends
                    turn = FIELD_OLED_TURN_THEIRS;
                    agent.message.type = MESSAGE_NONE;
                    agent.state = AGENT_STATE_DEFENDING;
                }
            } else {
                // no message if agent isn't in defending state
                agent.message.type = MESSAGE_NONE;
            }
            break;

        case BB_EVENT_MESSAGE_SENT:
            // when awaiting to send message and about to attack, generate agent's
            // shot guess for the opponent's field to register
            if (agent.state == AGENT_STATE_WAITING_TO_SEND) {
                // generate a shot guess that attempts to target opponent's boats
                FieldAIDecideGuess(&agent.opp);
                agent.message.type = MESSAGE_SHO;
                agent.message.param0 = agent.ownGuess.row;
                agent.message.param1 = agent.ownGuess.col;

                // update number of turns counted and transition to attacking state
                turnCount++;
                agent.state = AGENT_STATE_ATTACKING;
            } else {
                // no message if agent isn't in waiting state
                agent.message.type = MESSAGE_NONE;
            }
            break;

        case BB_EVENT_ERROR:
            // determine what kind of BB_Error event occurred
            switch (event.type) {
                case BB_ERROR_BAD_CHECKSUM:
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Bad checksum.\n");
                    OledUpdate();
                    break;

                case BB_ERROR_PAYLOAD_LEN_EXCEEDED:
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Payload length exceeded.\n");
                    OledUpdate();
                    break;

                case BB_ERROR_CHECKSUM_LEN_EXCEEDED:
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Checksum length exceeded.\n");
                    OledUpdate();
                    break;

                case BB_ERROR_CHECKSUM_LEN_INSUFFICIENT:
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Insufficient checksum length.\n");
                    OledUpdate();
                    break;

                case BB_ERROR_INVALID_MESSAGE_TYPE:
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Invalid message type.\n");
                    OledUpdate();
                    break;

                case BB_ERROR_MESSAGE_PARSE_FAILURE:
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Message parse failure.\n");
                    OledUpdate();
                    break;

                default:
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Default error.\n");
                    OledUpdate();
                    break;
            }

            // an error occurred, so end game
            agent.message.type = MESSAGE_ERROR;
            agent.state = AGENT_STATE_END_SCREEN;
            return agent.message;
            break;

        case BB_EVENT_SOUTH_BUTTON:
            // no message or event since not considering extra credit
            agent.message.type = MESSAGE_NONE;
            break;

        case BB_EVENT_EAST_BUTTON:
            // no message or event since not considering extra credit
            agent.message.type = MESSAGE_NONE;
            break;
    }

    // update OLED with agent's field, opponent's field, whose turn it is, and turn count
    OledClear(OLED_COLOR_BLACK);
    FieldOledDrawScreen(&agent.own, &agent.opp, turn, turnCount);
    OledUpdate();
    return agent.message;
}

/********************************************************************************
 * Defined AgentGetState function.
 ********************************************************************************/
AgentState AgentGetState(void) {
    // send current state of agent
    return agent.state;
}

/********************************************************************************
 * Defined AgentSetState function.
 ********************************************************************************/
void AgentSetState(AgentState newState) {
    // set the new state as the current state of agent
    agent.state = newState;
}