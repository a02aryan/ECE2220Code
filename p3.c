/*
Name: Aryan Patel
CUID: ASP6
Course: ECE2220
Semester: Spring 2025
Assignment number: Project 3
Purpose: Encode/Decode using a 21-bit Hamming Code
Assumptions: None
Known Bugs: None
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

void printCode24(unsigned int codeword) { // prints 24-bit codeword, dashes for unused bits
    int i;
    for(i = 23; i >= 0; i--) { // loop from bit 23 to 0
        if(i >= 21) {
            printf("-"); // unused bits shown as dashes
        }
        else {
            printf("%d", (codeword >> i) & 1); // print actual bit
        }
        if(i % 4 == 0 && i != 0) {
            printf(" "); // add space every 4 bits
        }
    }
    printf("\n"); // new line at end
}

void printByte(unsigned int val, int size) { // prints binary representation of 'val'
    int i;
    for(i = size; i >= 0; i--) { // loop from 'size' to 0
        printf("%d", (val >> i) & 1); // print bit value
        if(i == 8 || i == 16) {
            printf(" "); // group bytes
        }
    }
    printf(" "); // extra space at end
}

void printCarets(void) { // prints carets under data bits, skipping parity positions
    int bPosition;
    for(bPosition = 23; bPosition >= 0; bPosition--) { // loop through 24 bits
        if(bPosition >= 21) {
            printf(" "); // skip unused bits
        }
        else {
            int bp = bPosition + 1;
            if(bp == 1 || bp == 2 || bp == 4 || bp == 8 || bp == 16) {
                printf(" "); // no caret for parity bits
            }
            else {
                printf("^"); // caret for data bit
            }
        }
        if(bPosition % 4 == 0 && bPosition != 0) {
            printf(" "); // space every 4 bits
        }
    }
    printf("\n"); // new line
}

void printParityBits(unsigned int codeword) {//prints parity bits
    int parity_positions[5] = {1, 2, 4, 8, 16};
    printf("Parity bits: ");
    
    for (int i = 0; i < 5; i++) {
        int parityPos = parity_positions[i];
        int parityBit = (codeword >> (parityPos - 1)) & 1; // Extract the parity bit
        printf("P%d=%d ", parityPos, parityBit);
    }
    
    printf("\n");
}

void encode(char line[]) { // encodes two chars into a 21-bit Hamming code
    char input[10], extra[10];
    char first, second;
    unsigned int binNum, codeword;
    int i, bPosition, BitIndex, bit;
    int parity_positions[5] = {1,2,4,8,16};

    if(sscanf(line, "%9s %c %c %9s", input, &first, &second, extra) != 3) {
        printf("Invalid encode input. Format: encode <char1> <char2>\n"); // check input
        return;
    }

    printf("\n                         %c        %c\n", second, first); // header with swapped order
    printf("0x    %02X       %02X       %02X       %02X\n", 0, 0, (unsigned char)second, (unsigned char)first); // print hex header
    binNum = (((unsigned int)second) << 8) | ((unsigned int)first); // combine chars into 16-bit number
    printf("-------- -------- ");
    printByte((unsigned int)second, 7);
    printByte((unsigned int)first, 7);
    printf("\n"); // binary header

    codeword = 0;
    BitIndex = 0;
    for(bPosition = 1; bPosition <= 21; bPosition++) { // insert data bits into codeword, skipping parity spots
        if(bPosition != 1 && bPosition != 2 && bPosition != 4 && bPosition != 8 && bPosition != 16) {
            bit = (binNum >> BitIndex) & 1; // get bit from binNum
            codeword |= (bit << (bPosition - 1)); // set bit in codeword
            BitIndex++; // next bit from binNum
        }
    }

    for(i = 0; i < 5; i++) { // compute parity bits
        int parityPos = parity_positions[i];
        int bitSum = 0;
        for(bPosition = 1; bPosition <= 21; bPosition++) {//sum bits
            if(bPosition != parityPos) {
                if(bPosition & parityPos) {
                    int currentBit = (codeword >> (bPosition - 1)) & 1;//get bit
                    bitSum += currentBit;
                }
            }
        }
        if(bitSum % 2 != 0) {
            int parityBitValue = 1 << (parityPos - 1);//set parity bit
            codeword |= parityBitValue;
        }
        else {
            int clearMask = ~(1 << (parityPos - 1));//clear parity bit
            codeword &= clearMask;
        }
    }
    printf("\n");
    printParityBits(codeword);
    printf("\n-------- ---");
    printByte(codeword, 20);
    printf("\n");
    printf("0x%02X%02X%02X%02X\n", (codeword >> 24) & 0xFF, (codeword >> 16) & 0xFF, (codeword >> 8) & 0xFF, codeword & 0xFF); // print hex codeword
}

void decode(char line[]) { // decodes a 6-digit hex codeword back into two chars
    char input[10], hexStr[10], extra[10];
    unsigned int codeword, binNum;
    int i, bPosition, BitIndex, bit;
    int valid;

    if(sscanf(line, "%9s %9s %9s", input, hexStr, extra) != 2) {
        printf("Invalid decode input. Format: decode <6_hex_digits>\n"); // check input
        return;
    }
    if(strlen(hexStr) != 6) {
        printf("Invalid decode input. The hex codeword must be exactly 6 characters.\n"); // check length
        return;
    }

    valid = 1;
    i = 0;
    while(i < 6 && valid == 1) {
        if(!isxdigit(hexStr[i])) {
            valid = 0; // validate hex digits
        }
        i++;
    }
    if(valid == 0) {//
        printf("Invalid decode input. The codeword must contain only hex digits.\n");
        return;
    }

    codeword = (unsigned int)strtoul(hexStr, NULL, 16); // convert hex to int/binary

    for(i = 0; i < 6; i++) {
        printf("   %c ", toupper(hexStr[i])); // print hex digits spaced
    }
    printf("\n");

    printCode24(codeword);
    printCarets();

    binNum = 0;
    BitIndex = 0;
    for(bPosition = 1; bPosition <= 21; bPosition++) { // extract data bits from codeword
        if(bPosition != 1 && bPosition != 2 && bPosition != 4 && bPosition != 8 && bPosition != 16) {//skip parity bits
            bit = (codeword >> (bPosition - 1)) & 1;//get bit from codeword
            binNum |= (bit << BitIndex);//set bit in binNum
            BitIndex++;
        }
    }

    printf("     ");
    for(i = 15; i >= 0; i--) {
        printf("%d", (binNum >> i) & 1);
        if(i % 4 == 0 && i != 0) {
            printf(" "); // print extracted binary
        }
    }
    printf("\n");

    printf("        %X    %X    %X    %X\n", (binNum >> 12) & 0xF, (binNum >> 8) & 0xF, (binNum >> 4) & 0xF, binNum & 0xF); // print hex nibbles

    {
        char ch1, ch2;
        ch1 = (char)(binNum & 0xFF);
        ch2 = (char)((binNum >> 8) & 0xFF);
        printf("=         '%c'       '%c'\n", ch2, ch1); // print decoded chars
    }
    printf("\n");
}

int main(void) { // main loop
    char buf[100], input[10];
    int running = 0, len;

    while(running != 1) {
        printf("Enter one of these options: encode <char1> <char2> or decode <6_hex_digits> or exit\n");//prompt
        if(fgets(buf, sizeof(buf), stdin) != NULL) {//gets input
            len = strlen(buf);//get length of input
            if(len > 0 && buf[len-1] == '\n') {
                buf[len-1] = '\0';//remove newline
            }
            if(sscanf(buf, "%9s", input) == 1) {//get first word
                if(strcmp(input, "exit") == 0) {//check if exit
                    running = 1;
                }
                else if(strcmp(input, "encode") == 0) {//check if encode
                    encode(buf);
                }
                else if(strcmp(input, "decode") == 0) {//check if decode
                    decode(buf);
                }
                else {
                    printf("Unknown command. Use 'encode <char1> <char2>', 'decode <6_hex_digits>', or 'exit'.\n");//invalid command
                }
            }
        }
    }
    return 0;
}
