// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>

//CSE013E Support Library
#include "UNIXBOARD.h"

// User libraries
#include "Game.h"
#include "Player.h"


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****
struct GameInfo {
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
    char input;
    int inputCheck;
    uint8_t exits;
};

// **** Define any global or external variables here ****
static struct GameInfo gameInfo;
#define NO_GAME_ROOM_EXITS 0b0000 // switch case for checking whether a room has not exit

// **** Declare any function prototypes here ****


int main()
{



    /******************************** Your custom code goes below here ********************************/
    printf("Welcome to my DnD RPG!");
    
    // initialize the game to start it
    int game = GameInit();
    if (game == STANDARD_ERROR) {
        FATAL_ERROR();
    }

    while (1) {
        // obtain and print the title and description of room
        GameGetCurrentRoomTitle(gameInfo.title);
        GameGetCurrentRoomDescription(gameInfo.desc);
        printf("\n\n\n%s\n", gameInfo.title);
        printf("\n%s\n", gameInfo.desc);

        // get and print all possible exits in the room
        gameInfo.exits = GameGetCurrentRoomExits();
        printf("\nPossible exits: ");
        if (gameInfo.exits & GAME_ROOM_EXIT_NORTH_EXISTS) {
            printf("North ");
        }
        if (gameInfo.exits & GAME_ROOM_EXIT_EAST_EXISTS) {
            printf("East ");
        }
        if (gameInfo.exits & GAME_ROOM_EXIT_SOUTH_EXISTS) {
            printf("South ");
        }
        if (gameInfo.exits & GAME_ROOM_EXIT_WEST_EXISTS) {
            printf("West ");
        }
        if (!(gameInfo.exits | NO_GAME_ROOM_EXITS)) {
            printf("This room is a dead end. Press 'q' to quit.");
        }
        printf("\n\n");

        while (1) {
            // get the player's input for which direction to go and check if it's valid
            // by seeing if only one character was entered
            printf("Where do you want to go? (Enter 'n' for North, 'e' for East, "
                   "'s' for South, and 'w' for West or 'q' to quit.): ");
            gameInfo.input = getchar();
            while (getchar() != '\n') {
                if (getchar() == '\n') {
                    printf("Invalid input, try again.\n");
                }
            }

            // check the user input to see if the direction out of the current room
            // exists or if the player wants to quit
            switch (gameInfo.input) {
                case 'n':
                    gameInfo.inputCheck = GameGoNorth();
                    if (gameInfo.inputCheck == STANDARD_ERROR) {
                        printf("There is no North exit!\n");
                    }
                    break;

                case 'e':
                    gameInfo.inputCheck = GameGoEast();
                    if (gameInfo.inputCheck == STANDARD_ERROR) {
                        printf("There is no East exit!\n");
                    }
                    break;

                case 's':
                    gameInfo.inputCheck = GameGoSouth();
                    if (gameInfo.inputCheck == STANDARD_ERROR) {
                        printf("There is no South exit!\n");
                    }
                    break;

                case 'w':
                    gameInfo.inputCheck = GameGoWest();
                    if (gameInfo.inputCheck == STANDARD_ERROR) {
                        printf("There is no West exit!\n");
                    }
                    break;

                case 'q':
                    printf("You have quit the game.\n");
                    break;

                default:
                    printf("Invalid input, try again.\n");
                    break;
            }

            // see if the input check was successful or if the player wants to
            // quit in order to exit this loop, reset input check
            if (gameInfo.inputCheck || (gameInfo.input == 'q')) {
                gameInfo.inputCheck = 0;
                break;
            }
        }

        // end the game if the player inputs 'q'
        if (gameInfo.input == 'q') {
            exit(1);
        }
    }


    /**************************************************************************************************/
}

