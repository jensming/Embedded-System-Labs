/* 
 * File:   Negotiation.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on March 1, 2022, 11:27 PM
 */


// Standard libraries
#include <stdio.h>

// User libraries
#include "Negotiation.h"
#include "BOARD.h"


// **** Define any module-level, global, or external variables here ****
static NegotiationData hash; // holds hashed value of secret commitment
static uint32_t overflow; // calculated square of secret input modulo PUBLIC_KEY
static NegotiationData parity; // bit-parity of A XOR B


// **** Declared function prototypes ****
NegotiationData NegotiationHash(NegotiationData secret);
int NegotiationVerify(NegotiationData secret, NegotiationData commitment);
NegotiationOutcome NegotiateCoinFlip(NegotiationData A, NegotiationData B);
/********************************************************************************
 * Defined NegotiationHash function.
 ********************************************************************************/
NegotiationData NegotiationHash(NegotiationData secret) {
    // obtain the "beef hash" of the secret commitment
    overflow = ((uint32_t) secret * (uint32_t) secret) % PUBLIC_KEY;
    hash = overflow;
    return hash;
}

/********************************************************************************
 * Defined NegotiationVerify function.
 ********************************************************************************/
int NegotiationVerify(NegotiationData secret, NegotiationData commitment) {
    // verify if the hashed secret number matches the commitment hash
    if (NegotiationHash(secret) == commitment) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/********************************************************************************
 * Defined NegotiationCoinFlip function.
 ********************************************************************************/
NegotiationOutcome NegotiateCoinFlip(NegotiationData A, NegotiationData B) {
    // determine the outcome of coin flip by checking whether the modulo 2 of
    // parity is 1 for HEADS or 0 for TAILS
    parity = A ^ B;
    if ((parity % 2) == 1) {
        return HEADS;
    } else {
        return TAILS;
    }
}