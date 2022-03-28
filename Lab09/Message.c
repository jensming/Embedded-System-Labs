/* 
 * File:   Message.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on March 2, 2022, 11:38 PM
 */


// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// User libraries
#include "Message.h"
#include "BOARD.h"


// **** Declare any data types here ****
typedef enum {
    WAIT, PAYLOAD, CHECKSUM
} DecodeState;

// **** Define any module-level, global, or external variables here ****
static uint8_t checksum; // holds calculated checksum
static uint8_t checksumConv; // holds checksum_string conversion to numerical form
static char tempPayload[MESSAGE_MAX_PAYLOAD_LEN]; // temporarily holds payload
static char *token; // tokenized portions of payload
static const char delim[] = ","; // delimiter for tokenizing payload
static int expectedTokNum; // expected number of tokens
static char msgEncoded[MESSAGE_MAX_LEN]; // holds encoded message
static DecodeState decodeState = WAIT; // always start in WAIT state for decoding
static int index = 0; // always starts at zero for tracking index of payload/checksum
static char strChecksum[3]; // holds string of checksum
#define FIRST_DELIM '$' // payload delimiter
#define CHECKSUM_DELIM '*' // checksum delimiter
#define END_DELIM '\n' // end delimiter for message
#define CHECKSUM_LEN 2 // length restriction for checksum


// **** Declared function prototypes ****
uint8_t Message_CalculateChecksum(const char* payload);
int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event);
int Message_Encode(char *message_string, Message message_to_encode);
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event);

/********************************************************************************
 * Defined Message_CalculateChecksum function.
 ********************************************************************************/
uint8_t Message_CalculateChecksum(const char* payload) {
    // XOR each ASCII char in the payload to obtain the checksum
    checksum = 0;
    for (int i = 0; i < strlen(payload); i++) {
        checksum ^= payload[i];
    }
    return checksum;
}

/********************************************************************************
 * Defined Message_ParseMessage function.
 ********************************************************************************/
int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event) {
    // clear the parameters of message events
    message_event->param0 = 0;
    message_event->param1 = 0;
    message_event->param2 = 0;

    // see if checksum is two characters long
    if (strlen(checksum_string) != 2) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    // convert checksum_string to numeric form and then see if the payload's 
    // checksum matches the one passed in
    checksumConv = strtoul(checksum_string, NULL, 16);
    if (Message_CalculateChecksum(payload) != checksumConv) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    // copy the payload into a temporary variable and then tokenize it to obtain
    // the first portion of the payload
    strcpy(tempPayload, payload);
    token = strtok(tempPayload, delim);

    // check what the first token is to determine which BB_Event should happen,
    // also see if it's in the correct format or not
    if (strcmp(token, "CHA") == 0) {
        message_event->type = BB_EVENT_CHA_RECEIVED;
        expectedTokNum = 1;
    } else if (strcmp(token, "ACC") == 0) {
        message_event->type = BB_EVENT_ACC_RECEIVED;
        expectedTokNum = 1;
    } else if (strcmp(token, "REV") == 0) {
        message_event->type = BB_EVENT_REV_RECEIVED;
        expectedTokNum = 1;
    } else if (strcmp(token, "SHO") == 0) {
        message_event->type = BB_EVENT_SHO_RECEIVED;
        expectedTokNum = 2;
    } else if (strcmp(token, "RES") == 0) {
        message_event->type = BB_EVENT_RES_RECEIVED;
        expectedTokNum = 3;
    } else {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    // obtain the next tokens in payload depending on what the first token was
    for (int i = 0; i < expectedTokNum; i++) {
        token = strtok(NULL, delim);
        if (token == NULL) {
            message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }

        if (i == 0) {
            message_event->param0 = (uint16_t) atoi(token);
        } else if (i == 1) {
            message_event->param1 = (uint16_t) atoi(token);
        } else if (i == 2) {
            message_event->param2 = (uint16_t) atoi(token);
        }
    }

    // check for any unexpected tokens left as there shouldn't be from the looping
    token = strtok(NULL, delim);
    if (token != NULL) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    return SUCCESS;
}

/********************************************************************************
 * Defined Message_Encode function.
 ********************************************************************************/
int Message_Encode(char *message_string, Message message_to_encode) {
    // encode the message based on the type of payload
    switch (message_to_encode.type) {
        case MESSAGE_NONE:
            return 0;
            break;

        case MESSAGE_CHA:
            sprintf(tempPayload, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            checksum = Message_CalculateChecksum(tempPayload);
            sprintf(msgEncoded, MESSAGE_TEMPLATE, tempPayload, checksum);
            strcpy(message_string, msgEncoded);
            break;

        case MESSAGE_ACC:
            sprintf(tempPayload, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            checksum = Message_CalculateChecksum(tempPayload);
            sprintf(msgEncoded, MESSAGE_TEMPLATE, tempPayload, checksum);
            strcpy(message_string, msgEncoded);
            break;

        case MESSAGE_REV:
            sprintf(tempPayload, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            checksum = Message_CalculateChecksum(tempPayload);
            sprintf(msgEncoded, MESSAGE_TEMPLATE, tempPayload, checksum);
            strcpy(message_string, msgEncoded);
            break;

        case MESSAGE_SHO:
            sprintf(tempPayload, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0,
                    message_to_encode.param1);
            checksum = Message_CalculateChecksum(tempPayload);
            sprintf(msgEncoded, MESSAGE_TEMPLATE, tempPayload, checksum);
            strcpy(message_string, msgEncoded);
            break;

        case MESSAGE_RES:
            sprintf(tempPayload, PAYLOAD_TEMPLATE_RES, message_to_encode.param0,
                    message_to_encode.param1, message_to_encode.param2);
            checksum = Message_CalculateChecksum(tempPayload);
            sprintf(msgEncoded, MESSAGE_TEMPLATE, tempPayload, checksum);
            strcpy(message_string, msgEncoded);
            break;

        case MESSAGE_ERROR:
            return -1;
            break;
    }

    return strlen(message_string);
}

/********************************************************************************
 * Defined Message_Decode function.
 ********************************************************************************/
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event) {
    // decode the passed in char based on which part of the message it's in and 
    // set the BB_Event type along with param0 accordingly as no event should 
    // occur until the decoding process concludes
    switch (decodeState) {
        case WAIT:
            // decode for payload if '$' delimiter is the first char passed, if 
            // not, the message is invalid
            if (char_in == FIRST_DELIM) {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                decodeState = PAYLOAD;
            } else {
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                return STANDARD_ERROR;
            }
            break;

        case PAYLOAD:
            // keep adding payload to tempPayload until '*' delimiter is reached,
            // also check for format error cases (go back to WAIT if there are)
            if ((char_in == FIRST_DELIM) || (char_in == END_DELIM)) {
                index = 0;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                decodeState = WAIT;
                return STANDARD_ERROR;
            } else if (index > MESSAGE_MAX_PAYLOAD_LEN) {
                index = 0;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_PAYLOAD_LEN_EXCEEDED;
                decodeState = WAIT;
                return STANDARD_ERROR;
            } else if (char_in == CHECKSUM_DELIM) {
                tempPayload[index] = '\0';
                index = 0;
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                decodeState = CHECKSUM;
            } else {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                tempPayload[index] = char_in;
                index++;
            }
            break;

        case CHECKSUM:
            // add valid checksum to strChecksum until '\n' delimiter is reached,
            // account for format/length errors and return to WAIT state if
            // encountered or if checksum has been fully obtained
            if (char_in == END_DELIM) {
                if (index < CHECKSUM_LEN) {
                    index = 0;
                    decoded_message_event->type = BB_EVENT_ERROR;
                    decoded_message_event->param0 = BB_ERROR_CHECKSUM_LEN_INSUFFICIENT;
                    decodeState = WAIT;
                    return STANDARD_ERROR;
                } else {
                    strChecksum[index] = '\0';
                    index = 0;
                    if (Message_ParseMessage(tempPayload, strChecksum, decoded_message_event) == STANDARD_ERROR) {
                        decoded_message_event->type = BB_EVENT_ERROR;
                        decoded_message_event->param0 = BB_ERROR_MESSAGE_PARSE_FAILURE;
                        decodeState = WAIT;
                        return STANDARD_ERROR;
                    } else {
                        // successful decode if ending here!
                        decodeState = WAIT;
                    }
                }
            } else if ((char_in < '0') || ((char_in > '9') && (char_in < 'A')) || (char_in > 'F')) {
                index = 0;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_BAD_CHECKSUM;
                decodeState = WAIT;
                return STANDARD_ERROR;
            } else {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                strChecksum[index] = char_in;
                index++;
            }
            break;
    }

    return SUCCESS;
}