/* 
 * File:   MessageTest.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on March 4, 2022, 11:23 AM
 */


// Standard libraries
#include <stdio.h>
#include <string.h>

// User libraries
#include "Message.h"
#include "BOARD.h"
#include "Uart1.h"


// **** Define any module-level, global, or external variables here ****
#define MCCS 1 // used to only test Message_CalculateChecksum()
#define MPM 1 // used to only test Message_ParseMessage()
#define ME 1 // used to only test Messgage_Encode()
#define MD 1 // used to only test Message_Decode()
static char *payload; // points to payload
static uint8_t checksum; // points to checksum
static char *strChecksum; // string of checksum
static BB_Event BBEvent; // BattleBoats events
static int msgParsed; // holds result of parsed message string
static Message msgEvent; // Message events
static char msg[MESSAGE_MAX_LEN]; // holds result of encoded message
static char *msgDecode; // holds result of decoded message
static int decodedResult; // holds result of decoded message


int main() {
    BOARD_Init();

    printf("Testing Message functions:\n\n");

#if MCCS
    printf("Message_CalculateChecksum()\n");

    // testing challenge message checksum
    payload = "CHA,43";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x61) {
        printf("    Passed checksum test for CHA!\n");
    } else {
        printf("    Failed checksum test for CHA.\n");
    }

    // testing shot guess message checksum
    payload = "SHO,43,44";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x53) {
        printf("    Passed checksum test for SHO!\n");
    } else {
        printf("    Failed checksum test for SHO.\n");
    }

    // testing result message checksum
    payload = "RES,43,44,45";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x6E) {
        printf("    Passed checksum test for RES!\n");
    } else {
        printf("    Failed checksum test for RES.\n");
    }
#endif


#if MPM
    printf("Message_ParseMessage()\n");

    // testing parse message of challenge for correct population of BB_Event
    payload = "CHA,43";
    strChecksum = "61";
    msgParsed = Message_ParseMessage(payload, strChecksum, &BBEvent);
    if ((msgParsed == SUCCESS) && (BBEvent.type == BB_EVENT_CHA_RECEIVED) &&
            (BBEvent.param0 == 43)) {
        printf("    Passed parse message test for CHA!\n");
    } else {
        printf("    Failed parse message test for CHA.\n");
    }

    // testing parse message of shot guess for correct population of BB_Event
    payload = "SHO,43,44";
    strChecksum = "53";
    msgParsed = Message_ParseMessage(payload, strChecksum, &BBEvent);
    if ((msgParsed == SUCCESS) && (BBEvent.type == BB_EVENT_SHO_RECEIVED) &&
            (BBEvent.param0 == 43) && (BBEvent.param1 == 44)) {
        printf("    Passed parse message test for SHO!\n");
    } else {
        printf("    Failed parse message test for SHO.\n");
    }

    // testing parse message of result for correct population of BB_Event
    payload = "RES,43,44,45";
    strChecksum = "6E";
    msgParsed = Message_ParseMessage(payload, strChecksum, &BBEvent);
    if ((msgParsed == SUCCESS) && (BBEvent.type == BB_EVENT_RES_RECEIVED) &&
            (BBEvent.param0 == 43) && (BBEvent.param1 == 44) && (BBEvent.param2 == 45)) {
        printf("    Passed parse message test for RES!\n");
    } else {
        printf("    Failed parse message test for RES.\n");
    }

    // testing for checksum format error case
    strChecksum = "123";
    msgParsed = Message_ParseMessage(payload, strChecksum, &BBEvent);
    if ((msgParsed == STANDARD_ERROR) && (BBEvent.type == BB_EVENT_ERROR)) {
        printf("    Passed parse message checksum format error check!\n");
    } else {
        printf("    Failed parse message checksum format error check.\n");
    }

    // testing for payload format error case
    payload = "ERR,1";
    strChecksum = "16";
    msgParsed = Message_ParseMessage(payload, strChecksum, &BBEvent);
    if ((msgParsed == STANDARD_ERROR) && (BBEvent.type == BB_EVENT_ERROR)) {
        printf("    Passed parse message payload format error case!\n");
    } else {
        printf("    Failed parse message payload format error case.\n");
    }

    // testing for payload and checksum mismatch error case
    payload = "15";
    strChecksum = "16";
    msgParsed = Message_ParseMessage(payload, strChecksum, &BBEvent);
    if ((msgParsed == STANDARD_ERROR) && (BBEvent.type == BB_EVENT_ERROR)) {
        printf("    Passed parse message payload and checksum mismatch error check!\n");
    } else {
        printf("    Failed parse message payload and checksum mismatch error check.\n");
    }
#endif


#if ME
    printf("Message_Encode()\n");

    // testing for no message encoding
    msgEvent.type = MESSAGE_NONE;
    int msgNone = Message_Encode(msg, msgEvent);
    if (msgNone == 0) {
        printf("    Passed message encoding for no message!\n");
    } else {
        printf("    Failed message encoding for no message.\n");
    }

    // testing for challenge message encoding
    msgEvent.type = MESSAGE_CHA;
    msgEvent.param0 = 43;
    Message_Encode(msg, msgEvent);
    if (strcmp(msg, "$CHA,43*61")) {
        printf("    Passed message encoding for CHA!\n");
    } else {
        printf("    Failed message encoding for CHA.\n");
    }

    // testing for shot guess message encoding
    msgEvent.type = MESSAGE_SHO;
    msgEvent.param0 = 43;
    msgEvent.param1 = 44;
    Message_Encode(msg, msgEvent);
    if (strcmp(msg, "$SHO,43,44*53")) {
        printf("    Passed message encoding for SHO!\n");
    } else {
        printf("    Failed message encoding for SHO.\n");
    }

    // testing for result message encoding
    msgEvent.type = MESSAGE_RES;
    msgEvent.param0 = 43;
    msgEvent.param1 = 44;
    msgEvent.param2 = 45;
    Message_Encode(msg, msgEvent);
    if (strcmp(msg, "$RES,43,44,45*6E")) {
        printf("    Passed message encoding for RES!\n");
    } else {
        printf("    Failed message encoding for RES.\n");
    }
#endif


#if MD
    printf("Message_Decode()\n");

    // testing for challenge message decoding
    msgDecode = "$CHA,43*61\n";
    for (int i = 0; i < strlen(msgDecode); i++) {
        if (i == (strlen(msgDecode) - 1)) {
            decodedResult = Message_Decode(msgDecode[i], &BBEvent);
        } else {
            Message_Decode(msgDecode[i], &BBEvent);
        }
    }
    if ((decodedResult == SUCCESS) && (BBEvent.type == BB_EVENT_CHA_RECEIVED) &&
            (BBEvent.param0 == 43)) {
        printf("    Passed message decoding for CHA!\n");
    } else {
        printf("    Failed message decoding for CHA.\n");
    }

    // testing for shot guess message decoding
    msgDecode = "$SHO,43,44*53\n";
    for (int i = 0; i < strlen(msgDecode); i++) {
        if (i == (strlen(msgDecode) - 1)) {
            decodedResult = Message_Decode(msgDecode[i], &BBEvent);
        } else {
            Message_Decode(msgDecode[i], &BBEvent);
        }
    }
    if ((decodedResult == SUCCESS) && (BBEvent.type == BB_EVENT_SHO_RECEIVED) &&
            (BBEvent.param0 == 43) && (BBEvent.param1 == 44)) {
        printf("    Passed message decoding for SHO!\n");
    } else {
        printf("    Failed message decoding for SHO.\n");
    }

    // testing for result message decoding
    msgDecode = "$RES,43,44,45*6E\n";
    for (int i = 0; i < strlen(msgDecode); i++) {
        if (i == (strlen(msgDecode) - 1)) {
            decodedResult = Message_Decode(msgDecode[i], &BBEvent);
        } else {
            Message_Decode(msgDecode[i], &BBEvent);
        }
    }
    if ((decodedResult == SUCCESS) && (BBEvent.type == BB_EVENT_RES_RECEIVED) &&
            (BBEvent.param0 == 43) && (BBEvent.param1 == 44) && (BBEvent.param2 == 45)) {
        printf("    Passed message decoding for RES!\n");
    } else {
        printf("    Failed message decoding for RES.\n");
    }

    // testing for payload and checksum mismatch error case
    msgDecode = "$RES,43,44,45*6A\n";
    for (int i = 0; i < strlen(msgDecode); i++) {
        if (i == (strlen(msgDecode) - 1)) {
            decodedResult = Message_Decode(msgDecode[i], &BBEvent);
        } else {
            Message_Decode(msgDecode[i], &BBEvent);
        }
    }
    if ((decodedResult == STANDARD_ERROR) && (BBEvent.type == BB_EVENT_ERROR) &&
            (BBEvent.param0 == BB_ERROR_MESSAGE_PARSE_FAILURE)) {
        printf("    Passed message decoding payload and checksum mismatch error case!\n");
    } else {
        printf("    Failed message decoding payload and checksum mismatch error case.\n");
    }

    // testing for message decoding format error case
    msgDecode = "$RES,1*6A\n";
    for (int i = 0; i < strlen(msgDecode); i++) {
        if (i == (strlen(msgDecode) - 1)) {
            decodedResult = Message_Decode(msgDecode[i], &BBEvent);
        } else {
            Message_Decode(msgDecode[i], &BBEvent);
        }
    }
    if ((decodedResult == STANDARD_ERROR) && (BBEvent.type == BB_EVENT_ERROR) &&
            (BBEvent.param0 == BB_ERROR_MESSAGE_PARSE_FAILURE)) {
        printf("    Passed message decoding format error case!\n");
    } else {
        printf("    Failed message decoding format error case.\n");
    }
#endif

    BOARD_End();
    while (1);
}