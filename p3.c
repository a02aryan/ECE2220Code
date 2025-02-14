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

void printCode24(unsigned int codeword) {
    int i;
    for (i = 23; i >= 0; i--) {
        if (i >= 21)
            printf("-");
        else
            printf("%d", (codeword >> i) & 1);
        if (i % 4 == 0 && i != 0)
            printf(" ");
    }
    printf("\n");
}

void printByte(unsigned int val, int size) {
    int i;
    for (i = size; i >= 0; i--) {
        printf("%d", (val >> i) & 1);
        if (i == 8 || i == 16)
            printf(" ");
    }
    printf(" ");
}

void printCarets(void) {
    int bPosition;
    for (bPosition = 23; bPosition >= 0; bPosition--) {
        if (bPosition >= 21) {
            printf(" ");
        } else {
            int bp = bPosition + 1;
            if (bp == 1 || bp == 2 || bp == 4 || bp == 8 || bp == 16)
                printf(" ");
            else
                printf("^");
        }
        if (bPosition % 4 == 0 && bPosition != 0)
            printf(" ");
    }
    printf("\n");
}

void encode(char line[]) {
    char input[10], extra[10];
    char first, second;
    unsigned int binNum, codeword;
    int i, bPosition, BitIndex, bit;
    int parity_positions[5] = {1, 2, 4, 8, 16}, parityValues[5];

    if (sscanf(line, "%9s %c %c %9s", input, &first, &second, extra) != 3) {
        printf("Invalid encode input. Format: encode <char1> <char2>\n");
        return;
    }
    printf("\n                         %c        %c\n", second, first);
    printf("0x    %02X       %02X       %02X       %02X\n", 0, 0, (unsigned char)second, (unsigned char)first);
    binNum = (((unsigned int)second) << 8) | ((unsigned int)first);
    printf("-------- -------- ");
    printByte((unsigned int)second, 7);
    printByte((unsigned int)first, 7);
    printf("\n");
    codeword = 0;
    BitIndex = 0;
    for (bPosition = 1; bPosition <= 21; bPosition++) {
        if (bPosition != 1 && bPosition != 2 && bPosition != 4 && bPosition != 8 && bPosition != 16) {
            bit = (binNum >> BitIndex) & 1;
            codeword |= (bit << (bPosition - 1));
            BitIndex++;
        }
    }
    for (i = 0; i < 5; i++) {
        int parityPos = parity_positions[i];
        int bitSum = 0;
        for (bPosition = 1; bPosition <= 21; bPosition++) {
            if (bPosition != parityPos) {
                int maskResult = bPosition & parityPos;
                if (maskResult != 0) {
                    int currentBit = (codeword >> (bPosition - 1)) & 1;
                    bitSum = bitSum + currentBit;
                }
            }
        }
        if (bitSum % 2 != 0) {
            int parityBitValue = 1 << (parityPos - 1);
            int tempCodeword = codeword | parityBitValue;
            codeword = tempCodeword;
            parityValues[i] = 1;
        } else {
            int clearMask = ~(1 << (parityPos - 1));
            int tempCodeword = codeword & clearMask;
            codeword = tempCodeword;
            parityValues[i] = 0;
        }
    }    
    printf("\n");
    printf("-------- ---");
    printByte(codeword, 20);
    printf("\n\n");
    printf("0x%02X%02X%02X%02X\n", (codeword >> 24) & 0xFF, (codeword >> 16) & 0xFF, (codeword >> 8) & 0xFF, codeword & 0xFF);
}

void decode(char line[]) {
    char input[10], hexStr[10], extra[10];
    unsigned int codeword, binNum;
    int i, bPosition, BitIndex, bit;
    int valid;
    if (sscanf(line, "%9s %9s %9s", input, hexStr, extra) != 2) {
        printf("Invalid decode input. Format: decode <6_hex_digits>\n");
        return;
    }
    if (strlen(hexStr) != 6) {
        printf("Invalid decode input. The hex codeword must be exactly 6 characters.\n");
        return;
    }
    valid = 1;
    i = 0;
    while (i < 6 && valid == 1) {
        if (!isxdigit(hexStr[i])) {
            valid = 0;
        }
        i++;
    }
    if (valid == 0) {
        printf("Invalid decode input. The codeword must contain only hex digits.\n");
        return;
    }
    codeword = (unsigned int)strtoul(hexStr, NULL, 16);
    for (i = 0; i < 6; i++) {
        printf("   %c ", toupper(hexStr[i]));
    }
    printf("\n");
    printCode24(codeword);
    printCarets();
    binNum = 0;
    BitIndex = 0;
    for (bPosition = 1; bPosition <= 21; bPosition++) {
        if (bPosition != 1 && bPosition != 2 && bPosition != 4 && bPosition != 8 && bPosition != 16) {
            bit = (codeword >> (bPosition - 1)) & 1;
            binNum |= (bit << BitIndex);
            BitIndex++;
        }
    }
    printf("     ");
    for (i = 15; i >= 0; i--) {
        printf("%d", (binNum >> i) & 1);
        if (i % 4 == 0 && i != 0)
            printf(" ");
    }
    printf("\n");
    printf("        %X    %X    %X    %X\n", (binNum >> 12) & 0xF, (binNum >> 8) & 0xF, (binNum >> 4) & 0xF, binNum & 0xF);
    {
        char ch1, ch2;
        ch1 = (char)(binNum & 0xFF);
        ch2 = (char)((binNum >> 8) & 0xFF);
        printf("=         '%c'       '%c'\n", ch2, ch1);
    }
    printf("\n");
}

int main(void) {
    char buf[100], input[10];
    int running = 0, len;
    while (running != 1) {
        printf("Enter one of these options: encode <char1> <char2> or decode <6_hex_digits> or exit\n");
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n')
                buf[len-1] = '\0';
            if (sscanf(buf, "%9s", input) == 1) {
                if (strcmp(input, "exit") == 0)
                    running = 1;
                else if (strcmp(input, "encode") == 0)
                    encode(buf);
                else if (strcmp(input, "decode") == 0)
                    decode(buf);
                else
                    printf("Unknown command. Use 'encode <char1> <char2>', 'decode <6_hex_digits>', or 'exit'.\n");
            }
        }
    }
    return 0;
}
