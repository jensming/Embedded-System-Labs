/* 
 * File:   Game.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on February 23, 2022, 1:25 PM
 */

// Standard libraries
#include <stdio.h>
#include <string.h>

// User libraries
#include "Game.h"
#include "Player.h"


// **** Declare any datatypes here ****
struct Room {
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
    int northExit;
    int eastExit;
    int southExit;
    int westExit;
};

// **** Define any module-level, global, or external variables here ****
static struct Room room;
static uint8_t exits = 0b0000; // start off with no exits
static char openFileStr[30]; // holds full filename of room
static FILE *fp; // points to file to open
static int fileLen; // manipulated to get lengths of room properties
static uint8_t inventoryItem; // item that's required for a version of room
static uint8_t foundItem; // item that can be found in room
static int errorCheck; // checks for STANDARD_ERROR in openFile
static int roomVersion; // determines which version of a room is used
static int gameCheck; // checks to see if game was successfully initialized

// **** Put any helper functions here ****
int openFile(int roomNum) {
    // setting initial version of room
    roomVersion = 1;

    // obtain filename string with desired room number and then open it,
    // check if the open file is viable too
    sprintf(openFileStr, "RoomFiles/room%02d.txt", roomNum);
    fp = fopen(openFileStr, "r");
    if (fp == NULL) {
        FATAL_ERROR();
    }

    // skip the RPG portion of the file and then read the title portion to room.title, 
    // next make sure the title is NULL-terminated so that it can be used as a string
    fseek(fp, 3, SEEK_SET);
    fileLen = fgetc(fp);
    fread(room.title, fileLen, 1, fp);
    room.title[fileLen] = '\0';
    if (strlen(room.title) != fileLen) {
        return STANDARD_ERROR;
    }

    // obtain the number of items required to determine what version of room to 
    // display and then see if the player has such items in inventory
    fileLen = fgetc(fp);
    for (int i = fileLen; i > 0; i--) {
        inventoryItem = fgetc(fp);
        if (FindInInventory(inventoryItem) != SUCCESS) {
            roomVersion = 0;
            break;
        }
    }

    // read the description portion of the file to the room.desc, then make sure 
    // the description is NULL-terminated so that it can be used as a string
    fileLen = fgetc(fp);
    fread(room.desc, fileLen, 1, fp);
    room.desc[fileLen] = '\0';
    if (strlen(room.desc) != fileLen) {
        return STANDARD_ERROR;
    }

    // obtain number of items found in room and if it's not in the inventory
    // already, add the item to it if there's still space
    fileLen = fgetc(fp);
    for (int i = fileLen; i > 0; i--) {
        foundItem = fgetc(fp);
        if (FindInInventory(foundItem) != SUCCESS) {
            errorCheck = AddToInventory(foundItem);
            if (errorCheck == STANDARD_ERROR) {
                return STANDARD_ERROR;
            }
        }
    }

    // obtain existing exits in the room
    room.northExit = fgetc(fp);
    room.eastExit = fgetc(fp);
    room.southExit = fgetc(fp);
    room.westExit = fgetc(fp);

    // rewrite room properties to other version if player didn't meet the initial
    // required items for that particular version
    if (roomVersion == 0) {
        // check for any required items in this version of the room
        fileLen = fgetc(fp);
        for (int i = fileLen; i > 0; i--) {
            inventoryItem = fgetc(fp);
            if (FindInInventory(inventoryItem) != SUCCESS) {
                return STANDARD_ERROR;
            }
        }

        // read description of this room version to room.desc, then make sure 
        // the description is NULL-terminated so that it can be used as a string
        fileLen = fgetc(fp);
        fread(room.desc, fileLen, 1, fp);
        room.desc[fileLen] = '\0';
        if (strlen(room.desc) != fileLen) {
            return STANDARD_ERROR;
        }

        // obtain number of items found in this version of the room and if it's 
        // not in the inventory already, add the item to it if there's still space
        fileLen = fgetc(fp);
        for (int i = fileLen; i > 0; i--) {
            foundItem = fgetc(fp);
            if (FindInInventory(foundItem) != SUCCESS) {
                errorCheck = AddToInventory(foundItem);
                if (errorCheck == STANDARD_ERROR) {
                    return STANDARD_ERROR;
                }
            }
        }

        // obtain existing exits in this version of the room
        room.northExit = fgetc(fp);
        room.eastExit = fgetc(fp);
        room.southExit = fgetc(fp);
        room.westExit = fgetc(fp);
    }

    fclose(fp);
    return SUCCESS;
}

// **** Declared function prototypes ****
int GameGoNorth(void);
int GameGoEast(void);
int GameGoSouth(void);
int GameGoWest(void);
int GameInit(void);
int GameGetCurrentRoomTitle(char *title);
int GameGetCurrentRoomDescription(char *desc);
uint8_t GameGetCurrentRoomExits(void);


/********************************************************************************
 * Defined GameGoNorth function.
 ********************************************************************************/
int GameGoNorth(void) {
    // check if the north exit is there and if it is, open the file for that room
    if ((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS) && (openFile(room.northExit) == SUCCESS)) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/********************************************************************************
 * Defined GameGoEast function.
 ********************************************************************************/
int GameGoEast(void) {
    // check if the east exit is there and if it is, open the file for that room
    if ((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS) && (openFile(room.eastExit) == SUCCESS)) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/********************************************************************************
 * Defined GameGoSouth function.
 ********************************************************************************/
int GameGoSouth(void) {
    // check if the south exit is there and if it is, open the file for that room
    if ((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS) && (openFile(room.southExit) == SUCCESS)) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/********************************************************************************
 * Defined GameGoWest function.
 ********************************************************************************/
int GameGoWest(void) {
    // check if the west exit is there and if it is, open the file for that room
    if ((GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS) && (openFile(room.westExit) == SUCCESS)) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/********************************************************************************
 * Defined GameInit function.
 ********************************************************************************/
int GameInit(void) {
    // initialize the game by opening the file for the starting room
    gameCheck = openFile(STARTING_ROOM);
    if (gameCheck == SUCCESS) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/********************************************************************************
 * Defined GameGetCurrentRoomTitle function.
 ********************************************************************************/
int GameGetCurrentRoomTitle(char *title) {
    // copy the title of the current room into title and then obtain the length of it
    strcpy(title, room.title);
    return strlen(title);
}

/********************************************************************************
 * Defined GameGetCurrentRoomDescription function.
 ********************************************************************************/
int GameGetCurrentRoomDescription(char *desc) {
    // copy the description of the current room into desc and then obtain the length of it
    strcpy(desc, room.desc);
    return strlen(desc);
}

/********************************************************************************
 * Defined GameGetCurrentRoomExits function.
 ********************************************************************************/
uint8_t GameGetCurrentRoomExits(void) {
    // determine if the north, east, south, and west exits in the current room exist
    exits = 0b0000;
    if (room.northExit) {
        exits |= GAME_ROOM_EXIT_NORTH_EXISTS;
    }
    if (room.eastExit) {
        exits |= GAME_ROOM_EXIT_EAST_EXISTS;
    }
    if (room.southExit) {
        exits |= GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    if (room.westExit) {
        exits |= GAME_ROOM_EXIT_WEST_EXISTS;
    }
    return exits;
}