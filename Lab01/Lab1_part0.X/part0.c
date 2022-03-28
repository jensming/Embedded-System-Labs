/* 
 * File:   part0.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on January 3, 2022, 5:54 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"

/*
 * 
 */
int main(void) 
{
    BOARD_Init();
    printf("Hello World\n");
    
    BOARD_End(); // handles cleanup of the system
    while(1); // if embedded we should never exit
}

