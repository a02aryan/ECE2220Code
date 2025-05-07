/*
Name: Aryan Patel
CUID: ASP6
Course: ECE2220
Semester: Spring 2025
Assignment number: Project 4
Purpose: This program encrypts or decrypts a message using the Vigenere Cipher
Assumptions: None
Known Bugs: None
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 32 // Maximum length of the message
#define MAX_BUF 10 // Maximum length of the buffer

void removeWhitespace(const char *ptrStr, char *output) { // Function to remove whitespace from the string

    int len = strlen(ptrStr);
    int pos = 0;

    for (int i = 0; i < len; i++) { // Removing whitespace from the string
        if (!isspace((unsigned char)ptrStr[i])) { // Checking if the character is not a whitespace
            output[pos] = ptrStr[i];
            pos = pos + 1;
        }
    }
    output[pos] = '\0'; // Null terminating the string
}

void encode(const char *ptrinputMessage, const char *ptrcipherKey, char *output) { // Function to encode the message
    int letterCount = 0;
    int letterPos = 0;
    char tempMessage[MAX_LEN + 1];
    int inLen = strlen(ptrinputMessage);
    int keyLen = strlen(ptrcipherKey);

    for (int i = 0; i < inLen; i++) { // Removing whitespace and converting to uppercase
        if (!isspace((unsigned char)ptrinputMessage[i])) { // Checking if the character is not a whitespace
            tempMessage[letterCount] = (char)toupper((unsigned char)ptrinputMessage[i]);
            letterCount = letterCount + 1;
        }
    }

    tempMessage[letterCount] = '\0'; // Null terminating the temporary string

    for (int i = 0; i < letterCount; i++) { // Encoding each letter using the Vigenere Cipher
        int chrVal, keyVal, newVal;
        chrVal = tempMessage[i] - 'A';
        keyVal = ptrcipherKey[i % keyLen] - 'A';
        newVal = (chrVal + keyVal) % 26;
        tempMessage[i] = (char)(newVal + 'A');
    }

    for (int i = 0; i < inLen; i++) { // Rebuilding the final message with preserved whitespace
        if (isspace((unsigned char)ptrinputMessage[i])) { // Checking if the character is a whitespace
            output[i] = ptrinputMessage[i];
        }
        else {
            output[i] = tempMessage[letterPos];
            letterPos = letterPos + 1;
        }
    }
    output[inLen] = '\0'; // adding Null at the end of the output string
}

void decode(const char *ptrinputMessage, const char *ptrcipherKey, char *output) { // Function to decode the message
    int letterCount = 0;
    int letterPos = 0;
    char tempMessage[MAX_LEN + 1];
    int inLen = strlen(ptrinputMessage);
    int keyLen = strlen(ptrcipherKey);

    for (int i = 0; i < inLen; i++) { // Removing whitespace and converting to uppercase
        if (!isspace((unsigned char)ptrinputMessage[i])) { // Checking if the character is not a whitespace
            tempMessage[letterCount] = (char)toupper((unsigned char)ptrinputMessage[i]);
            letterCount = letterCount + 1;
        }
    }

    tempMessage[letterCount] = '\0'; // Null terminating the temporary string
    
    for (int i = 0; i < letterCount; i++) { // Decoding each letter using the Vigenere Cipher
        int chrVal, keyVal, newVal;
        chrVal = tempMessage[i] - 'A';
        keyVal = ptrcipherKey[i % keyLen] - 'A';
        newVal = (chrVal - keyVal + 26) % 26;
        tempMessage[i] = (char)(newVal + 'A');
    }

    for (int i = 0; i < inLen; i++) { // adding whitespace back
        if (isspace((unsigned char)ptrinputMessage[i])) { // Checking if the character is a whitespace
            output[i] = ptrinputMessage[i];
        }
        else {
            output[i] = tempMessage[letterPos];
            letterPos = letterPos + 1;
        }
    }
    output[inLen] = '\0'; // adding Null to the output string
}

int main(int argc, char *ptrargv[]) { // Main function
    char Buf[MAX_BUF];
    char result[MAX_LEN + 1];
    char keyBuffer[MAX_LEN + 1];
    char mesBuffer[MAX_LEN + 1];
    int pos = 0;

    if (argc != 4) {
        printf("Invalid Input \nEnter: ./prog4 <encode/decode> \"message\" \"code-phrase\"\n");
        return 1;
    }

    for (int i = 0; ptrargv[1][i] && i < (int)sizeof(Buf) - 1; i++) { // Converting the argument to lowercase
        Buf[i] = (char)tolower((unsigned char)ptrargv[1][i]);
        pos = pos + 1;
    }

    Buf[pos] = '\0'; // adding Null to buffer

    removeWhitespace(ptrargv[3], keyBuffer); // Removing whitespace from the key

    for (int i = 0; keyBuffer[i] != '\0'; i++) { // Converting the key to uppercase
        keyBuffer[i] = (char)toupper((unsigned char)keyBuffer[i]);
    }

    if (strlen(keyBuffer) > MAX_LEN) { // Checking if the key is valid
        printf("Error: Code phrase is more then 32 characters.\n");
        return 1;
    }

    removeWhitespace(ptrargv[2], mesBuffer); // Removing whitespace from the message
    if (strlen(mesBuffer) > MAX_LEN) { // Checking if the message is valid
        printf("Error: Message is more then 32 characters.\n");
        return 1;
    }
    
    if (strcmp(Buf, "encode") == 0) { // Checking if encode is typed
        encode(ptrargv[2], keyBuffer, result);
    }
    else if (strcmp(Buf, "decode") == 0) { // Checking if decode is typed
        decode(ptrargv[2], keyBuffer, result);
    }
    else {
        printf("Invalid operation. The first input should be Encode or Decode \n");
        return 1;
    }
    printf("%s\n", result); // Printing the final result
    return 0;
}
