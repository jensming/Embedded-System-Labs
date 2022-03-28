/* 
 * File:   NegotiationTest.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on March 1, 2022, 11:45 PM
 */

// Standard libraries
#include <stdio.h>

// User libraries
#include "Negotiation.h"
#include "BOARD.h"
#include "Uart1.h"


// **** Define any module-level, global, or external variables here ****
#define NH 1 // used to only test NegotiationHash()
#define NV 1 // used to only test NegotiationVerify()
#define NCF 1 // used to only test NegotiateCoinFlip()
NegotiationData secret; // holds secret number for tests


int main() {
    BOARD_Init();

    printf("Testing Negotiation functions:\n\n");

#if NH
    printf("NegotiationHash()\n");

    // testing for correct hashing functionality with basic value
    secret = 15;
    if (NegotiationHash(secret) == 225) {
        printf("    Passed NegotiationHash() of basic value.\n");
    } else {
        printf("    Failed NegotiationHash() of basic value.\n");
    }

    // testing for correct hashing functionality with zero value
    secret = 0;
    if (NegotiationHash(secret) == 0) {
        printf("    Passed NegotiationHash() of zero value.\n");
    } else {
        printf("    Failed NegotiationHash() of zero value.\n");
    }

    // testing for correct hashing functionality with overflow value
    secret = 0xFFFF;
    if (NegotiationHash(secret) == 34011) {
        printf("    Passed NegotiationHash() of overflow value.\n");
    } else {
        printf("    Failed NegotiationHash() of overflow value.\n");
    }
#endif


#if NV
    printf("NegotiationVerify()\n");

    // testing for correct hash verification functionality with basic value and cheat attempt
    secret = 15;
    NegotiationData cheat = 30;
    if ((NegotiationVerify(secret, 225) == TRUE) && (NegotiationVerify(secret, cheat) == FALSE)) {
        printf("    Passed NegotiationVerify() of basic value along with cheat attempt.\n");
    } else {
        printf("    Failed NegotiationVerify() of basic value along with cheat attempt.\n");
    }

    // testing for correct hash verification functionality with zero value
    secret = 0;
    if (NegotiationVerify(secret, 0) == TRUE) {
        printf("    Passed NegotiationVerify() of zero value.\n");
    } else {
        printf("    Failed NegotiationVerify() of zero value.\n");
    }

    // testing for correct hash verification functionality with overflow value
    secret = 0xFFFF;
    if (NegotiationVerify(secret, 34011) == TRUE) {
        printf("    Passed NegotiationVerify() of overflow value.\n");
    } else {
        printf("    Failed NegotiationVerify() of overflow value.\n");
    }
#endif


#if NCF
    printf("NegotiatonCoinFlip()\n");

    // 12 XOR 15 = 3, which is an odd number that should mean the coin is on HEADS
    secret = 12;
    if (NegotiateCoinFlip(secret, 15) == HEADS) {
        printf("    Passed NegotiateCoinFlip() in determining HEADS.\n");
    } else {
        printf("    Failed NegotiateCoinFlip() in determining HEADS.\n");
    }

    // 15 XOR 21 = 26, which is an even number that should mean the coin is on TAILS
    secret = 15;
    if (NegotiateCoinFlip(secret, 21) == TAILS) {
        printf("    Passed NegotiateCoinFlip() in determining TAILS.\n");
    } else {
        printf("    Failed NegotiateCoinFlip() in determining TAILS.\n");
    }
#endif

    BOARD_End();
    while (1);
}