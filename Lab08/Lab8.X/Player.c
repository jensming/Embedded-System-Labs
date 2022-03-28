/* 
 * File:   Player.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on February 23, 2022, 1:27 PM
 */

// Standard libraries
#include <stdio.h>

// User libraries
#include "Player.h"


// **** Define any module-level, global, or external variables here ****
static uint8_t playerInventory[INVENTORY_SIZE]; // array that holds player's inventory items
static int currentItemIndex = 0; // index of player's current inventory item

// **** Declared function prototypes ****
int AddToInventory(uint8_t item);
int FindInInventory(uint8_t item);


/********************************************************************************
 * Defined AddToInventory function.
 ********************************************************************************/
int AddToInventory(uint8_t item) {
    if (currentItemIndex < INVENTORY_SIZE) {
        // add item to player inventory if there's still room left
        playerInventory[currentItemIndex] = item;
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/********************************************************************************
 * Defined FindInInventory function.
 ********************************************************************************/
int FindInInventory(uint8_t item) {
    // check for the desired item by iterating backwards through the inventory
    for (int i = currentItemIndex; i >= 0; i--) {
        if (playerInventory[i] == item) {
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}