/*
Name: Aryan Patel
CUID: ASP6
Course: ECE2220
Semester: Spring 2025
Assignment number: Project 5
Purpose: This program implements the Zen Word Game
Assumptions: None
Known Bugs: None
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define DICT_BLOCK_SIZE 5000 // Number of words to read at a time
#define INIT_SIZE 100 // Initial size of the dictionary
#define MAX_WORD_LENGTH 100// Maximum length of a word / helps with buffer overflow
#define MIN_WORD_LENGTH 3/// Minimum length of a word
#define MAX_GROUP 5// Maximum number of groups


void toUppercase(char *str) { // Function to convert a string to uppercase
    int i = 0;
    while (str[i] != '\0') {
        str[i] = toupper(str[i]);
        i++;
    }
}

void letterPicker(char *ptrLetters) {// Function to pick letters for the game
    srand(time(NULL));
    
    char vowels[] = "AEIOU"; 
    int vowelsize = sizeof(vowels) - 1;
    
    char consonants[] = "BCDFGHKLMNPRST";
    int consonantSize = sizeof(consonants) - 1;
    
    int letterIndex = 0;
    
    int FirstVowel = 0;
    while (FirstVowel == 0) {// Loop to pick the first vowel
        int vowelIndex = rand() % vowelsize;
        ptrLetters[letterIndex] = vowels[vowelIndex];
        letterIndex++;
        FirstVowel = 1;
    }
    
    
    int SecondVowel = 0;
    while (SecondVowel == 0) {// Loop to pick the second vowel
        int vowelIndex = rand() % vowelsize; // Randomly pick a vowel
        
        
        int Duplicate = 0;
        for (int i = 0; i < letterIndex; i++) { // Check for duplicates
            if (ptrLetters[i] == vowels[vowelIndex]) { 
                Duplicate = 1;
            }
        }
        
        if (Duplicate == 0) { // If no duplicates, add the vowel
            ptrLetters[letterIndex] = vowels[vowelIndex];
            letterIndex++;
            SecondVowel = 1;
        }
    }
    
    while (letterIndex < 7) { // Loop to pick the remaining letters
        int consonantIndex = rand() % consonantSize;
         
        int Duplicate = 0;
        for (int i = 0; i < letterIndex; i++) { // Check for duplicates
            if (ptrLetters[i] == consonants[consonantIndex]) {
                Duplicate = 1;
            }
        }
        
        if (Duplicate == 0) { // If no duplicates, add the consonant
            ptrLetters[letterIndex] = consonants[consonantIndex];
            letterIndex++;
        }
    }
        
    ptrLetters[7] = '\0';
}

char **openDict(const char *fileName, int *wordCount) { // Function to open the dictionary file
    FILE *ptrFile = fopen(fileName, "r");
    if (!ptrFile) {
        printf("Dictionary file not found\n");
        return NULL;
    }

    printf("Reading dictionary");
    
    int maxSize = INIT_SIZE;
    char **ptrDict = malloc(maxSize * sizeof(char *)); // Allocate memory for the dictionary
    if (ptrDict == NULL) {
        printf("Call to malloc() unsuccessful.\n");
        return NULL;
    }

    *wordCount = 0;
    char word[MAX_WORD_LENGTH];
    int index = 0;
    char c;

    while (fread(&c, 1, 1, ptrFile) == 1) { // Read the file character by character
        if (isalpha(c)) { // Check if the character is a letter
            if (index < MAX_WORD_LENGTH - 1)
                word[index++] = c;
        } else if (index > 0) {// If a word is formed, store it
            word[index] = '\0';
            if (*wordCount >= maxSize) {// Check if the dictionary is full
                maxSize += 1;;
                ptrDict = realloc(ptrDict, maxSize * sizeof(char *));
            }
            ptrDict[*wordCount] = malloc(strlen(word) + 1);// Allocate memory for the word
            if (ptrDict[*wordCount] == NULL) {
                printf("Call to malloc() unsuccessful.\n");
                return NULL;
            }
            if (ptrDict[*wordCount]) // Copy the word to the dictionary
                strcpy(ptrDict[*wordCount], word);
            (*wordCount)++;
            if ((*wordCount) % DICT_BLOCK_SIZE == 0)// Print progress
                printf(".");
            index = 0;
        }
    }
    printf("\n");
    fclose(ptrFile);

    return ptrDict;
}

void printlayout(char *ptrLetters) {// Function to print the layout of the letters
    printf("      %c\n", toupper(ptrLetters[0]));
    printf("   %c     %c\n", toupper(ptrLetters[1]), toupper(ptrLetters[2]));
    printf("  %c       %c\n", toupper(ptrLetters[3]), toupper(ptrLetters[4]));
    printf("    %c   %c\n", toupper(ptrLetters[5]), toupper(ptrLetters[6]));
}

char **findValidWords(char **dictionary, int wordCount, char *ptrLetters, int *foundCount) { // Function to find valid words
    char upperLetters[8];
    for (int i = 0; i < 7; i++) {// Convert letters to uppercase
        upperLetters[i] = toupper(ptrLetters[i]);
    }
    upperLetters[7] = '\0';
    
    int max = 10;
    char **ptrResult = malloc(max * sizeof(char *));// Allocate memory for the valid words
    if (ptrResult == NULL) {
        printf("Call to malloc() unsuccessful.\n");
        return NULL;
    }

    *foundCount = 0;
    
    for (int i = 0; i < wordCount; i++) {// Loop through the dictionary
        char wordBuffer[MAX_WORD_LENGTH];
        strncpy(wordBuffer, dictionary[i], MAX_WORD_LENGTH - 1);// Copy the word to the buffer
        wordBuffer[MAX_WORD_LENGTH - 1] = '\0';
        
        toUppercase(wordBuffer);// Convert the word to uppercase
        
        int len = strlen(wordBuffer);
        if (len >= 3 && len <= 7) {// Check if the word is valid
            int valid = 1;
            for (char c = 'A'; c <= 'Z'; c++) {
                int wordOccurrence = 0;
                for (int j = 0; j < len; j++) {
                    if (wordBuffer[j] == c) {
                        wordOccurrence++;
                    }
                }
                
                int letterOccurrence = 0;
                for (int j = 0; j < 7; j++) {
                    if (upperLetters[j] == c) {
                        letterOccurrence++;
                    }
                }
                if (wordOccurrence > letterOccurrence) {
                    valid = 0;
                }
            }
            if (valid == 1) {
                if (*foundCount >= max) {// Check if the result array is full
                    max += 1;
                    ptrResult = realloc(ptrResult, max * sizeof(char *));// Allocate more memory for the result array
                }
                ptrResult[*foundCount] = malloc((len + 1) * sizeof(char));// Allocate memory for the word
                if (ptrResult[*foundCount] == NULL) {
                    printf("Call to malloc() unsuccessful.\n");
                    return NULL;
                }
                strcpy(ptrResult[*foundCount], wordBuffer);// Copy the word to the result array
                (*foundCount)++;
            }
        }
    }
    return ptrResult;// Return the result array
}

void printBoard(char ***ptrWordLists, int *ptrWordListSizes, int **ptrGuessed) {// Function to print the board
    int totalWords = 0;
    for (int i = 0; i < MAX_GROUP; i++) {// Loop through the word lists
        totalWords += ptrWordListSizes[i];
    }
    int columns = 4;
    int rows = (totalWords + columns - 1) / columns;
    
    for (int r = 0; r < rows; r++) {// Loop through the rows
        for (int c = 0; c < columns; c++) {// Loop through the columns
            int wordCounter = 0;
            int targetGroup = -1;
            int targetPos = -1;
            
            for (int g = 0; g < MAX_GROUP; g++) {// Loop through the groups
                for (int p = 0; p < ptrWordListSizes[g]; p++) {// Loop through the positions
                    if (wordCounter == c * rows + r) {// Check if the word is in the current position
                        targetGroup = g;
                        targetPos = p;
                    }
                    wordCounter++;
                }
            }
            
            if (targetGroup >= 0 && targetPos >= 0) {// Check if the word is valid
                if (ptrGuessed[targetGroup][targetPos]) {
                    printf("%s    ", ptrWordLists[targetGroup][targetPos]);
                } else {// If the word is not guessed, print the first letter and dashes
                    int len = strlen(ptrWordLists[targetGroup][targetPos]);
                    printf("%c", ptrWordLists[targetGroup][targetPos][0]);
                    for (int k = 1; k < len; k++) {// Print dashes for the remaining letters
                        printf("-");
                    }
                    printf("    ");
                }
            }
        }
        printf("\n");
    }
}

void buildList(char **validWords, int validCount,char ***ptrWordLists, int *ptrWordListSizes, int **ptrGuessed, int *ptrTotalValid) {// Function to build the word lists
    printf("Creating word lists");
    int *ptrMaxGroup = malloc(MAX_GROUP * sizeof(int));// Allocate memory for the maximum group sizes
    if (ptrMaxGroup == NULL) {//
        printf("Call to malloc() unsuccessful.\n");
        return; //since void not returning anything
    }
    
    for (int i = 0; i < MAX_GROUP; i++) {// Loop through the groups
        ptrWordListSizes[i] = 0;
        ptrMaxGroup[i] = 10;
        ptrWordLists[i] = malloc(ptrMaxGroup[i] * sizeof(char *));// Allocate memory for the word lists
        ptrGuessed[i] = calloc(ptrMaxGroup[i], sizeof(int));// Allocate memory for the guessed array
        if (ptrWordLists[i] == NULL || ptrGuessed[i] == NULL) {
            printf("Call to malloc() unsuccessful.\n");
            return; //since void not returning anything
        }
    }

    *ptrTotalValid = 0;
    int progressCounter = 0;
    int progress = validCount / 20;// Calculate progress
    if (progress < 1) {// round progress up
        progress = 1;
    }

    for (int i = 0; i < validCount; i++) {// Loop through the valid words
        int len = strlen(validWords[i]);
        if (len >= MIN_WORD_LENGTH && len <= MIN_WORD_LENGTH + MAX_GROUP - 1) {// Check if the word is valid
            int group = len - MIN_WORD_LENGTH;
            if (ptrWordListSizes[group] >= ptrMaxGroup[group]) {// Check if the group is full
                ptrMaxGroup[group]++;// Increase the size of the group
                ptrWordLists[group] = realloc(ptrWordLists[group], ptrMaxGroup[group] * sizeof(char *));// Reallocate memory
                ptrGuessed[group] = realloc(ptrGuessed[group], ptrMaxGroup[group] * sizeof(int));// Reallocate memory
            }
            ptrWordLists[group][ptrWordListSizes[group]] = malloc((len + 1) * sizeof(char));// Allocate memory for the word
            if (ptrWordLists[group][ptrWordListSizes[group]] == NULL) {
                printf("Call to malloc() unsuccessful.\n");
                return;
            }
            strcpy(ptrWordLists[group][ptrWordListSizes[group]], validWords[i]);// Copy the word to the group
            ptrGuessed[group][ptrWordListSizes[group]] = 0;// Initialize the guessed array
            ptrWordListSizes[group]++;
            (*ptrTotalValid)++;// Increase the total valid words count
        }
        progressCounter++;
        if (progressCounter >= progress) {// Print progress
            printf(".");
            progressCounter = 0;
        }
    }
    
    printf("\n");
    free(ptrMaxGroup);// Free the memory
}

int markGuessed(char ***ptrWordLists, int *ptrWordListSizes, int **ptrGuessed, char *guess) {// Function to mark a word as guessed
    int found = 0;

    for (int group = 0; group < MAX_GROUP; group++) {// Loop through the groups
        for (int j = 0; j < ptrWordListSizes[group]; j++) {// Loop through the positions
            if (strcmp(ptrWordLists[group][j], guess) == 0) {
                if (!ptrGuessed[group][j]) {// Check if the word is already guessed
                    ptrGuessed[group][j] = 1;
                    found = 1;
                }
            }
        }
    }

    return found;
}

int guessedWord(int *ptrWordListSizes, int **ptrGuessed) {// Function to check if all words are guessed
    int complete = 1;

    for (int group = 0; group < MAX_GROUP; group++) {// Loop through the groups
        for (int j = 0; j < ptrWordListSizes[group]; j++) {// Loop through the positions
            if (!ptrGuessed[group][j])// Check if the word is guessed
                complete = 0;
        }
    }
    
    return complete;// Return 1 if all words are guessed, 0 otherwise
}

int gamePlay(char ***ptrWordLists, int *ptrWordListSizes, int **ptrGuessed, char *ptrLetters) {// Function to play the game
    char input[MAX_WORD_LENGTH];
    int running = 1;
    int quit = 0;
    
    while (running == 1) {// Loop until the game is over
        printf("\nEnter a word or 'Q' to quit: ");
        
        if (fgets(input, MAX_WORD_LENGTH, stdin) != NULL) {// Read the input
            int len = strlen(input);
            if (len > 0 && input[len-1] == '\n') {// Check for newline character
                input[len-1] = '\0';
                len--;
            }
            
            char shortInput[MAX_WORD_LENGTH];
            int index2 = 0;// Index for shortInput
            int nonSpace = 0;//variable that stores check for non-space characters
            
            for (int i = 0; i < len; i++) {// Loop through the input
                if (!isspace(input[i])) {// Check for spaces
                    shortInput[index2++] = input[i];
                    nonSpace = 1;
                }
            }
            shortInput[index2] = '\0';// Null-terminate the string
            
            if (nonSpace) {// Check if there are non-space characters
                toUppercase(shortInput);
                
                if (strcmp(shortInput, "Q") == 0) {// Check for quit command
                    running = 0;
                    quit = 1;
                } else if (markGuessed(ptrWordLists, ptrWordListSizes, ptrGuessed, shortInput)) {// Check if the word is guessed
                        printf("Good guess!\n");
                } else { // prints "X" for each letter in the word
                    int xCount = strlen(shortInput);
                    if (xCount < 3){
                        xCount = 3;
                    }
                    if (xCount > 7) {
                        xCount = 7;
                    }
                    
                    for (int i = 0; i < xCount; i++) {
                        printf("X ");
                    }
                    printf("\n");
                }
                
                if (running == 1){// Check if the game is still running
                    printlayout(ptrLetters);// Print the layout of the letters
                    printBoard(ptrWordLists, ptrWordListSizes, ptrGuessed);// Print the board
                    if (guessedWord(ptrWordListSizes, ptrGuessed)) {// Check if all words are guessed
                        printf("Congratulations, you've guessed all words!\n");
                        running = 0;// End the game / prints new board with different letters later in the code
                    }
                }
            }
        }
    }
    
    return quit;// Return 1 if the game is quit, 0 otherwise
}

int main(int argc, char *ptrargv[]) {// Main function
    if (argc != 2) {
        printf("Not enough arguments.\nCommand should be: ./p5 dictionary\n");
        return 1;
    }

    int wordCount = 0;
    char **dictionary = openDict(ptrargv[1], &wordCount);// Open the dictionary file

    int running = 1;
    while (running == 1) {// Loop until the game is over
        char *ptrLetters = malloc(8 * sizeof(char));// Allocate memory for the letters
        if (ptrLetters == NULL) {
            printf("Call to malloc() unsuccessful.\n");
            return 1;
        }

        letterPicker(ptrLetters);// Pick letters for the game
        printlayout(ptrLetters);// Print the layout of the letters

        int validCount = 0;
        char **validWords = findValidWords(dictionary, wordCount, ptrLetters, &validCount);// Find valid words
        int ptrTotalValid = 0;

        char ***ptrWordLists = malloc(MAX_GROUP * sizeof(char **));// Allocate memory for the word lists
        int *ptrWordListSizes = malloc(MAX_GROUP * sizeof(int));// Allocate memory for the word list sizes
        int **ptrGuessed = malloc(MAX_GROUP * sizeof(int *));// Allocate memory for the guessed array
        if (ptrWordLists == NULL || ptrWordListSizes == NULL || ptrGuessed == NULL) {
            printf("Call to malloc() unsuccessful.\n");
            return 1;
        }
        buildList(validWords, validCount, ptrWordLists, ptrWordListSizes, ptrGuessed, &ptrTotalValid);// Build the word lists
        printBoard(ptrWordLists, ptrWordListSizes, ptrGuessed);// Print the board
        
        int quitGame = gamePlay(ptrWordLists, ptrWordListSizes, ptrGuessed, ptrLetters);// Play the game
        if (quitGame == 1) {
            running = 0;
        }
        
        for (int i = 0; i < validCount; i++) {
            free(validWords[i]);// Free the valid words
        }
        free(validWords);// Free the valid words array
        for (int group = 0; group < MAX_GROUP; group++) {// Loop through the groups
            for (int j = 0; j < ptrWordListSizes[group]; j++) {// Loop through the positions
                free(ptrWordLists[group][j]);// Free the words
            }
            free(ptrWordLists[group]);// Free the word lists words
            free(ptrGuessed[group]);// Free the guessed words
        }
        free(ptrWordLists);// Free the word lists
        free(ptrGuessed);// Free the guessed array
        free(ptrWordListSizes);// Free the word list sizes
        free(ptrLetters);// Free the letters
    }

    for (int i = 0; i < wordCount; i++) {// Loop through the dictionary to free the words
        free(dictionary[i]);
    }
    free(dictionary);// Free the dictionary array(pointer to the dictionary)

    return 0;
}