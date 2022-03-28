/* 
 * File:   Field.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on March 6, 2022, 1:02 PM
 */


// Standard libraries
#include <stdio.h>
#include <string.h>
#include <time.h>

// User libraries
#include "Field.h"
#include "BOARD.h"


// **** Define any module-level, global, or external variables here ****
SquareStatus prevStatus; // holds previous value at a particular field location
SquareStatus squareType; // holds type of square at particular field location
static int boatLen; // holds length of boat that's being added to field
static uint8_t boatState; // tracks which boats are still alive in the field
static uint8_t smallCheck, mediumCheck, 
               largeCheck, hugeCheck; // tracks successful placement of boats in the field
static uint8_t rowRand, colRand, dirRand; // hold randomly generated numbers used in FieldAIPlaceAllBoats()
static GuessData guessData; // holds coordinates and result from a guess made
static int seed = 0; // seed for random number generator


// **** Declared function prototypes ****
void FieldPrint_UART(Field *own_field, Field * opp_field);
void FieldInit(Field *own_field, Field * opp_field);
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col);
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p);
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type);
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess);
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess);
uint8_t FieldGetBoatStates(const Field *f);
uint8_t FieldAIPlaceAllBoats(Field *own_field);
GuessData FieldAIDecideGuess(const Field *opp_field);

/********************************************************************************
 * Defined FieldPrint_UART function.
 ********************************************************************************/
void FieldPrint_UART(Field *own_field, Field * opp_field) {
    // print out agent's field
    for (int r = 0; r < FIELD_ROWS; r++) {
        for (int c = 0; c < FIELD_COLS; c++) {
            printf("%d ", own_field->grid[r][c]);
        }
        if (r == (FIELD_ROWS - 1)) {
            printf("\n\n");
        } else {
            printf("\n");
        }
    }

    // print out opponent's field
    for (int r = 0; r < FIELD_ROWS; r++) {
        for (int c = 0; c < FIELD_COLS; c++) {
            printf("%d ", opp_field->grid[r][c]);
        }
        printf("\n");
    }
}

/********************************************************************************
 * Defined FieldInit function.
 ********************************************************************************/
void FieldInit(Field *own_field, Field * opp_field) {
    // initialize both the agent and opponent fields' grids
    for (int r = 0; r < FIELD_ROWS; r++) {
        for (int c = 0; c < FIELD_COLS; c++) {
            own_field->grid[r][c] = FIELD_SQUARE_EMPTY;
            opp_field->grid[r][c] = FIELD_SQUARE_UNKNOWN;
        }
    }

    // initialize opponent's boat lives (will initialize agent's boat lives when 
    // adding boats to field)
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
}

/********************************************************************************
 * Defined FieldGetSquareStatus function.
 ********************************************************************************/
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col) {
    // specified field location has to be within the field bounds to be valid
    if (((row >= 0) && (row < FIELD_ROWS)) || ((col >= 0) && (col < FIELD_COLS))) {
        return f->grid[row][col];
    } else {
        return FIELD_SQUARE_INVALID;
    }
}

/********************************************************************************
 * Defined FieldSetSquareStatus function.
 ********************************************************************************/
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p) {
    // replace field location with new SquareStatus, return the old one
    prevStatus = f->grid[row][col];
    f->grid[row][col] = p;
    return prevStatus;
}

/********************************************************************************
 * Defined FieldAddBoat function.
 ********************************************************************************/
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type) {
    // check for valid row and col
    if ((row < 0) || (row >= FIELD_ROWS) || (col < 0) || (col >= FIELD_COLS)) {
        return STANDARD_ERROR;
    }

    // set variables that will hold the field square type and boat length/lives,
    // field square type is invalid and an error is returned otherwise
    squareType = FIELD_SQUARE_INVALID;
    boatLen = 0;
    switch (boat_type) {
        case FIELD_BOAT_TYPE_SMALL:
            squareType = FIELD_SQUARE_SMALL_BOAT;
            boatLen = FIELD_BOAT_SIZE_SMALL;
            own_field->smallBoatLives = boatLen;
            break;

        case FIELD_BOAT_TYPE_MEDIUM:
            squareType = FIELD_SQUARE_MEDIUM_BOAT;
            boatLen = FIELD_BOAT_SIZE_MEDIUM;
            own_field->mediumBoatLives = boatLen;
            break;

        case FIELD_BOAT_TYPE_LARGE:
            squareType = FIELD_SQUARE_LARGE_BOAT;
            boatLen = FIELD_BOAT_SIZE_LARGE;
            own_field->largeBoatLives = boatLen;
            break;

        case FIELD_BOAT_TYPE_HUGE:
            squareType = FIELD_SQUARE_HUGE_BOAT;
            boatLen = FIELD_BOAT_SIZE_HUGE;
            own_field->hugeBoatLives = boatLen;
            break;

        default:
            return STANDARD_ERROR;
    }

    if (dir == FIELD_DIR_SOUTH) {
        // add boat type to positions on the field going in the FIELD_DIR_SOUTH 
        // direction according to its size
        if ((row + boatLen - 1) < FIELD_ROWS) {
            for (int r = 0; r < boatLen; r++) {
                if (own_field->grid[row + r][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            for (int r = 0; r < boatLen; r++) {
                own_field->grid[row + r][col] = squareType;
            }
        } else {
            return STANDARD_ERROR;
        }
    } else if (dir == FIELD_DIR_EAST) {
        // add boat type to positions on the field going in the FIELD_DIR_EAST 
        // direction according to its size
        if ((col + boatLen - 1) < FIELD_COLS) {
            for (int c = 0; c < boatLen; c++) {
                if (own_field->grid[row][col + c] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            for (int c = 0; c < boatLen; c++) {
                own_field->grid[row][col + c] = squareType;
            }
        } else {
            return STANDARD_ERROR;
        }
    } else {
        return STANDARD_ERROR;
    }

    return SUCCESS;
}

/********************************************************************************
 * Defined FieldRegisterEnemyAttack function.
 ********************************************************************************/
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess) {
    // obtain value of own field position before it was attacked
    prevStatus = own_field->grid[opp_guess->row][opp_guess->col];

    // determine if a particular boat was hit or sunk, it's a miss otherwise
    switch (own_field->grid[opp_guess->row][opp_guess->col]) {
        case FIELD_SQUARE_SMALL_BOAT:
            own_field->smallBoatLives--;
            if (own_field->smallBoatLives > 0) {
                opp_guess->result = RESULT_HIT;
            } else {
                opp_guess->result = RESULT_SMALL_BOAT_SUNK;
            }
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            break;

        case FIELD_SQUARE_MEDIUM_BOAT:
            own_field->mediumBoatLives--;
            if (own_field->mediumBoatLives > 0) {
                opp_guess->result = RESULT_HIT;
                own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            } else {
                opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
            }
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            break;

        case FIELD_SQUARE_LARGE_BOAT:
            own_field->largeBoatLives--;
            if (own_field->largeBoatLives > 0) {
                opp_guess->result = RESULT_HIT;
            } else {
                opp_guess->result = RESULT_LARGE_BOAT_SUNK;
            }
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            break;

        case FIELD_SQUARE_HUGE_BOAT:
            own_field->hugeBoatLives--;
            if (own_field->hugeBoatLives > 0) {
                opp_guess->result = RESULT_HIT;
            } else {
                opp_guess->result = RESULT_HUGE_BOAT_SUNK;
            }
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            break;

        default:
            opp_guess->result = RESULT_MISS;
            break;
    }

    return prevStatus;
}

/********************************************************************************
 * Defined FieldUpdateKnowledge function.
 ********************************************************************************/
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess) {
    // obtain value of opponent's field position before it was attacked
    prevStatus = opp_field->grid[own_guess->row][own_guess->col];

    // determine if own guess is a hit or miss, plus check if a boat sinks due to hit
    switch (opp_field->grid[own_guess->row][own_guess->col]) {
        case FIELD_SQUARE_SMALL_BOAT:
            opp_field->smallBoatLives--;
            if ((opp_field->smallBoatLives > 0) || (opp_field->smallBoatLives == 0)) {
                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            } else {
                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            }
            //            if ((own_guess->result == RESULT_SMALL_BOAT_SUNK) || (own_guess->result == RESULT_HIT))
            //            {
            //                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            //            }
            //            else
            //            {
            //                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            //            }
            break;

        case FIELD_SQUARE_MEDIUM_BOAT:
            opp_field->mediumBoatLives--;
            if ((opp_field->mediumBoatLives > 0) || (opp_field->mediumBoatLives == 0)) {
                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            } else {
                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            }
            //            if ((own_guess->result == RESULT_MEDIUM_BOAT_SUNK) || (own_guess->result == RESULT_HIT))
            //            {
            //                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            //            }
            //            else
            //            {
            //                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            //            }
            break;

        case FIELD_SQUARE_LARGE_BOAT:
            opp_field->largeBoatLives--;
            if ((opp_field->largeBoatLives > 0) || (opp_field->largeBoatLives == 0)) {
                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            } else {
                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            }
            //            if ((own_guess->result == RESULT_LARGE_BOAT_SUNK) || (own_guess->result == RESULT_HIT))
            //            {
            //                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            //            }
            //            else
            //            {
            //                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            //            }
            break;

        case FIELD_SQUARE_HUGE_BOAT:
            opp_field->hugeBoatLives--;
            if ((opp_field->hugeBoatLives > 0) || (opp_field->hugeBoatLives == 0)) {
                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            } else {
                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            }
            //            if ((own_guess->result == RESULT_HUGE_BOAT_SUNK) || (own_guess->result == RESULT_HIT))
            //            {
            //                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            //            }
            //            else
            //            {
            //                opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            //            }
            break;

        default:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            break;
    }

    return prevStatus;
}

/********************************************************************************
 * Defined FieldGetBoatStates function.
 ********************************************************************************/
uint8_t FieldGetBoatStates(const Field *f) {
    // initialize state of boats
    boatState = 0;

    // check which boats are still alive in the field
    if (f->hugeBoatLives > 0) {
        boatState |= FIELD_BOAT_STATUS_HUGE;
    }
    if (f->largeBoatLives > 0) {
        boatState |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->mediumBoatLives > 0) {
        boatState |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->smallBoatLives > 0) {
        boatState |= FIELD_BOAT_STATUS_SMALL;
    }

    return boatState;
}

/********************************************************************************
 * Defined FieldAIPlaceAllBoats function.
 ********************************************************************************/
uint8_t FieldAIPlaceAllBoats(Field *own_field) {
    // set boat placement checks to FALSE initially, will become SUCCESS when boat
    // is correctly placed in field
    hugeCheck = FALSE;
    largeCheck = FALSE;
    mediumCheck = FALSE;
    smallCheck = FALSE;

    // set seed for random number generation
    //    srand(time(NULL));

    // loop with boat placements until all are correctly added to the field
    while ((smallCheck == FALSE) || (smallCheck == SUCCESS)) {
        rowRand = rand() % FIELD_ROWS;
        colRand = rand() % FIELD_COLS;
        dirRand = rand() % 2;

        if (hugeCheck == FALSE) {
            hugeCheck = FieldAddBoat(own_field, rowRand, colRand, dirRand, FIELD_BOAT_TYPE_HUGE);
        } else if ((hugeCheck == SUCCESS) && (largeCheck == FALSE)) {
            largeCheck = FieldAddBoat(own_field, rowRand, colRand, dirRand, FIELD_BOAT_TYPE_LARGE);
        } else if ((largeCheck == SUCCESS) && (mediumCheck == FALSE)) {
            mediumCheck = FieldAddBoat(own_field, rowRand, colRand, dirRand, FIELD_BOAT_TYPE_MEDIUM);
        } else if ((mediumCheck == SUCCESS) && (smallCheck == FALSE)) {
            smallCheck = FieldAddBoat(own_field, rowRand, colRand, dirRand, FIELD_BOAT_TYPE_SMALL);
        } else if (smallCheck == SUCCESS) {
            return SUCCESS;
        }
    }

    // problem occurred if returning here
    return STANDARD_ERROR;
}

/********************************************************************************
 * Defined FieldAIDecideGuess function.
 ********************************************************************************/
GuessData FieldAIDecideGuess(const Field *opp_field) {
    // set seed for random number generation
    seed++;
    srand(rand() + seed);

    // loop until valid coordinate guess has been made
    while (1) {
        rowRand = rand() % FIELD_ROWS;
        colRand = rand() % FIELD_COLS;

        if (opp_field->grid[rowRand][colRand] == FIELD_SQUARE_UNKNOWN) {
            guessData.row = rowRand;
            guessData.col = colRand;
            //            guessData.result = RESULT_MISS;
            break;
        }
    }

    return guessData;
}