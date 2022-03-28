/* 
 * File:   FieldTest.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on March 8, 2022, 12:49 AM
 */


// Standard libraries
#include <stdio.h>
#include <string.h>

// User libraries
#include "Field.h"
#include "BOARD.h"
#include "Uart1.h"


// **** Define any module-level, global, or external variables here ****
#define FPI 1 // used to test FieldPrint_UART() and FieldInit()
#define FGS 1 // used to test FieldGetSquareStatus() and FieldSetSquareStatus()
#define FAB 1 // used to test FieldAddBoat()
#define FAPAB 1 // used to test FieldPlaceAllBoats()
#define FGP 1 // used to test remaining field gameplay functions
static Field agent; // own field to test
static Field opponent; // opponent field to test
static GuessData oppGuess; // opponent's guess
static SquareStatus fieldStatus; // holds status of particular field square


int main() {
    BOARD_Init();

    printf("Testing Field functions:\n\n");

#if FPI
    printf("FieldInit() and FieldPrint_UART()\n");

    // testing to see if fields are initialized properly
    FieldInit(&agent, &opponent);
    printf("Result should print agent's field with FIELD_SQUARE_EMPTY squares and "
            "opponent's field with FIELD_SQUARE_UNKOWN squares:\n\n");
    FieldPrint_UART(&agent, &opponent);
#endif

#if FGS
    printf("FieldGetSquareStatus() and FieldSetSquareStatus()\n");

    // testing for getting and setting squares on the field
    FieldSetSquareStatus(&agent, 2, 3, FIELD_SQUARE_MEDIUM_BOAT);
    FieldSetSquareStatus(&agent, 3, 3, FIELD_SQUARE_LARGE_BOAT);
    if ((FieldGetSquareStatus(&agent, 2, 3) == FIELD_SQUARE_MEDIUM_BOAT) &&
            (FieldGetSquareStatus(&agent, 3, 3) == FIELD_SQUARE_LARGE_BOAT)) {
        printf("    Both are working!\n\n");
    } else {
        printf("    One or both aren't working.\n\n");
    }
    FieldSetSquareStatus(&agent, 2, 3, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(&agent, 3, 3, FIELD_SQUARE_EMPTY);
#endif

#if FAB
    printf("FieldAddBoat()\n");

    // testing to see if various boats were added to the field correctly
    FieldAddBoat(&agent, 5, 2, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(&agent, 4, 4, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(&agent, 0, 1, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(&agent, 0, 4, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE);
    FieldAddBoat(&agent, 1, 7, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE);
    printf("Result should print a horizontal small boat starting at (5, 2), a "
            "vertical medium boat starting at (0, 1), and a horizontal huge boat "
            "starting at (0, 4) while every field square remaining still empty:\n\n");
    FieldPrint_UART(&agent, &opponent);
    FieldInit(&agent, &opponent);
#endif

#if FAPAB
    printf("FieldAIPlaceAllBoats()\n");

    // testing to see if all boats were placed correctly in field
    if (FieldAIPlaceAllBoats(&agent) == SUCCESS) {
        printf("    Successfully placed all boats in field!\n\n");
        FieldPrint_UART(&agent, &opponent);
    } else {
        printf("    Failed to correctly place all boats in field.\n\n");
    }
#endif

#if FGP
    printf("FieldRegisterEnemyAttack() and FieldAIDecideGuess()\n");

    // testing for valid AI guess and functioning registration of enemy attack
    oppGuess = FieldAIDecideGuess(&opponent);
    fieldStatus = FieldRegisterEnemyAttack(&agent, &oppGuess);
    if (((fieldStatus == FIELD_SQUARE_EMPTY) && (oppGuess.result == RESULT_MISS)) ||
            (fieldStatus != FieldGetSquareStatus(&agent, oppGuess.row, oppGuess.col))) {
        printf("    Both functions are working!\n\n");
    } else {
        printf("    Something isn't working.\n\n");
    }

    printf("FieldUpdateKnowledge() and FieldGetBoatStates()\n");

    // testing to see if correct boat states were returned and if field knowledge
    // was updated correctly
    fieldStatus = FieldUpdateKnowledge(&opponent, &oppGuess);
    if ((fieldStatus != FieldGetSquareStatus(&opponent, oppGuess.row, oppGuess.col)) &&
            (FieldGetBoatStates(&agent) == 0b1111)) {
        printf("    Both functions are working!\n\n");
    } else {
        printf("    Something went wrong.\n\n");
    }
#endif
    BOARD_End();
    while (1);
}