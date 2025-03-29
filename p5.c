#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_GROUP 5

void letterPicker(char *letters) {
    // Set seed for random number generation
    srand(time(NULL));
    
    // Define vowels array
    char vowels[] = "AEIOU";
    int vowelCount = sizeof(vowels) - 1; // -1 for the null terminator
    
    // Define common consonants (omitting Z, J, Q, X, V)
    char consonants[] = "BCDFGHKLMNPRST";
    int consonantCount = sizeof(consonants) - 1; // -1 for the null terminator
    
    // Pick 2 unique vowels
    int letterIndex = 0;
    int usedVowelIndices[2] = {-1, -1};
    
    // First vowel
    int vowelIndex = rand() % vowelCount;
    letters[letterIndex] = vowels[vowelIndex];
    usedVowelIndices[0] = vowelIndex;
    letterIndex++;
    
    // Second vowel - ensure it's different from the first
    int secondVowelFound = 0;
    while (secondVowelFound == 0) {
        vowelIndex = rand() % vowelCount;
        if (vowelIndex != usedVowelIndices[0]) {
            letters[letterIndex] = vowels[vowelIndex];
            usedVowelIndices[1] = vowelIndex;
            letterIndex++;
            secondVowelFound = 1;
        }
    }
    
    int usedConsonantIndices[5] = {-1, -1, -1, -1, -1};
    
    for (int i = 0; i < 5; i++) {
        int uniqueConsonantFound = 0;
        
        while (uniqueConsonantFound == 0) {
            int consonantIndex = rand() % consonantCount;
            
            int isDuplicate = 0;
            for (int j = 0; j < i; j++) {
                if (consonantIndex == usedConsonantIndices[j]) {
                    isDuplicate = 1;
                }
            }
            
            if (isDuplicate == 0) {
                letters[letterIndex] = consonants[consonantIndex];
                usedConsonantIndices[i] = consonantIndex;
                letterIndex++;
                uniqueConsonantFound = 1;
            }
        }
    }
    
    letters[7] = '\0';
    
    printf("Letters: ");
    for (int i = 0; i < 7; i++) {
        printf("%c ", letters[i]);
    }
    printf("\n");
}

char **openDict(const char *fileName, int *wordCount) {
    FILE *ptrFile = fopen(fileName, "r");
    if (!ptrFile) {
        printf("Dictionary file not found\n");
        return NULL;
    }
    printf("Reading dictionary");
    int maxSize = 100;
    char **dict = malloc(maxSize * sizeof(char *));
    *wordCount = 0;
    char word[MAX_WORD_LENGTH];
    int indx = 0;
    char ch;
    while (fread(&ch, 1, 1, ptrFile) == 1) {
        if (isalpha(ch)) {
            if (indx < MAX_WORD_LENGTH - 1)
                word[indx++] = ch;
        } else {
            if (indx > 0) {
                word[indx] = '\0';
                if (*wordCount >= maxSize) {
                    maxSize *= 2;
                    dict = realloc(dict, maxSize * sizeof(char *));
                }
                dict[*wordCount] = malloc(strlen(word) + 1);
                if (dict[*wordCount])
                    strcpy(dict[*wordCount], word);
                (*wordCount)++;
                if ((*wordCount) % 5000 == 0)
                    printf(".");
                indx = 0;
            }
        }
    }
    if (indx > 0) {
        word[indx] = '\0';
        if (*wordCount >= maxSize) {
            maxSize *= 2;
            dict = realloc(dict, maxSize * sizeof(char *));
        }
        dict[*wordCount] = malloc(strlen(word) + 1);
        if (dict[*wordCount])
            strcpy(dict[*wordCount], word);
        (*wordCount)++;
        if ((*wordCount) % 5000 == 0)
            printf(".");
    }
    printf("\nNumber of Words: %d\n", *wordCount);
    fclose(ptrFile);
    return dict;
}

void printlayout(char *letters) {
    printf("      %c\n", toupper(letters[0]));
    printf("   %c     %c\n", toupper(letters[1]), toupper(letters[2]));
    printf("  %c       %c\n", toupper(letters[3]), toupper(letters[4]));
    printf("    %c   %c\n", toupper(letters[5]), toupper(letters[6]));
}

char **findValidWords(char **dictionary, int wordCount, char *letters, int *foundCount) {
    char upperLetters[8];
    for (int i = 0; i < 7; i++) {
        upperLetters[i] = toupper(letters[i]);
    }
    upperLetters[7] = '\0';
    int letterFreq[26] = {0};
    for (int i = 0; i < 7; i++) {
        letterFreq[upperLetters[i] - 'A']++;
    }
    int max = 10;
    char **result = malloc(max * sizeof(char *));
    *foundCount = 0;
    for (int i = 0; i < wordCount; i++) {
        char temp[MAX_WORD_LENGTH];
        strncpy(temp, dictionary[i], MAX_WORD_LENGTH - 1);
        temp[MAX_WORD_LENGTH - 1] = '\0';
        toupper(temp);
        int len = strlen(temp);
        if (len >= 3 && len <= 7) {
            int tempFreq[26];
            memcpy(tempFreq, letterFreq, sizeof(tempFreq));
            int valid = 1;
            int j = 0;
            for (; j < len && valid; j++) {
                int idx = temp[j] - 'A';
                tempFreq[idx]--;
                if (tempFreq[idx] < 0) {
                    valid = 0;
                }
            }
            if (valid) {
                if (*foundCount >= max) {
                    max *= 2;
                    result = realloc(result, max * sizeof(char *));
                }
                result[*foundCount] = malloc((len + 1) * sizeof(char));
                strcpy(result[*foundCount], temp);
                (*foundCount)++;
            }
        }
    }
    return result;
}

void printBoard(char ***wordLists, int *wordListSizes, int **guessed) {
    int totalWords = 0;
    for (int i = 0; i < MAX_GROUP; i++) {
        totalWords += wordListSizes[i];
    }
    int columns = 4;
    int rows = (totalWords + columns - 1) / columns;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            int idx = c * rows + r;
            if (idx < totalWords) {
                int group = 0;
                int pos = idx;
                while (group < MAX_GROUP && pos >= wordListSizes[group]) {
                    pos = pos - wordListSizes[group];
                    group = group + 1;
                }
                if (group < MAX_GROUP) {
                    if (guessed[group][pos]) {
                        printf("%s    ", wordLists[group][pos]);
                    } else {
                        int len = strlen(wordLists[group][pos]);
                        printf("%c", wordLists[group][pos][0]);
                        for (int k = 1; k < len; k++) {
                            printf("-");
                        }
                        printf("    ");
                    }
                }
            }
        }
        printf("\n");
    }
}

void buildList(char **validWords, int validCount,
                          char ****wordListsPtr, int **wordListSizesPtr, int ***guessedPtr, int *totalValid) {
    char ***wordLists = malloc(MAX_GROUP * sizeof(char **));
    int *wordListSizes = malloc(MAX_GROUP * sizeof(int));
    int *wordListCaps = malloc(MAX_GROUP * sizeof(int));
    int **guessed = malloc(MAX_GROUP * sizeof(int *));
    for (int i = 0; i < MAX_GROUP; i++) {
        wordListSizes[i] = 0;
        wordListCaps[i] = 10;
        wordLists[i] = malloc(wordListCaps[i] * sizeof(char *));
        guessed[i] = calloc(wordListCaps[i], sizeof(int));
    }
    *totalValid = 0;
    for (int i = 0; i < validCount; i++) {
        int len = strlen(validWords[i]);
        if (len >= 3 && len <= 3 + MAX_GROUP - 1) {
            int group = len - 3;
            if (wordListSizes[group] >= wordListCaps[group]) {
                wordListCaps[group] = wordListCaps[group] * 2;
                wordLists[group] = realloc(wordLists[group], wordListCaps[group] * sizeof(char *));
                guessed[group] = realloc(guessed[group], wordListCaps[group] * sizeof(int));
            }
            wordLists[group][wordListSizes[group]] = malloc((len + 1) * sizeof(char));
            strcpy(wordLists[group][wordListSizes[group]], validWords[i]);
            guessed[group][wordListSizes[group]] = 0;
            wordListSizes[group] = wordListSizes[group] + 1;
            *totalValid = *totalValid + 1;
        }
    }
    *wordListsPtr = wordLists;
    *wordListSizesPtr = wordListSizes;
    *guessedPtr = guessed;
    free(wordListCaps);
}

int markGuessed(char ***wordLists, int *wordListSizes, int **guessed, char *guess) {
    int found = 0;
    for (int group = 0; group < MAX_GROUP; group++) {
        for (int j = 0; j < wordListSizes[group]; j++) {
            if (strcmp(wordLists[group][j], guess) == 0) {
                if (!guessed[group][j]) {
                    guessed[group][j] = 1;
                    found = 1;
                }
            }
        }
    }
    return found;
}

int guessedWord(int *wordListSizes, int **guessed) {
    int complete = 1;
    for (int group = 0; group < MAX_GROUP; group++) {
        for (int j = 0; j < wordListSizes[group]; j++) {
            if (!guessed[group][j])
                complete = 0;
        }
    }
    return complete;
}

void gamePlay(char ***wordLists, int *wordListSizes, int **guessed, char *letters) {
    char input[MAX_WORD_LENGTH];
    int running = 1;
    while (running == 1) {
        printf("\nEnter a word or 'Q' to quit: ");
        scanf("%s", input);
        toUppercase(input);
        if (strcmp(input, "Q") == 0) {
            running = 0;
        } else {
            if (markGuessed(wordLists, wordListSizes, guessed, input)) {
                printf("Good guess!\n");
            } else {
                printf("X X X X X\n");
            }
        }
        if (running == 1){
            printlayout(letters);
            printf("\nWord Board:\n");
            printBoard(wordLists, wordListSizes, guessed);
            if (guessedWord(wordListSizes, guessed)) {
                printf("Congratulations, you've guessed all words!\n");
                running = 0;
            }
        }
    }
}

int main(int argc, char *ptrargv[]) {
    if (argc != 2) {
        printf("Not enough arguments.\nCommand should be: ./p5 dictionary\n");
        return 1;
    }

    char *letters = malloc(8 * sizeof(char));
    if (!letters) {
        printf("Memory allocation failed for letters.\n");
        return 1;
    }

    int wordCount = 0;
    char **dictionary = openDict(ptrargv[1], &wordCount);
    if (dictionary == NULL) {
        free(letters);
        return 1;
    }

    letterPicker(letters);
    printlayout(letters);

    int validCount = 0;
    char **validWords = findValidWords(dictionary, wordCount, letters, &validCount);
    char ***wordLists;
    int *wordListSizes;
    int **guessed;
    int totalValid = 0;

    buildList(validWords, validCount, &wordLists, &wordListSizes, &guessed, &totalValid);
    printf("\nWord Board:\n");
    printBoard(wordLists, wordListSizes, guessed);
    gamePlay(wordLists, wordListSizes, guessed, letters);
    for (int i = 0; i < wordCount; i++) {
        free(dictionary[i]);
    }
    free(dictionary);
    for (int i = 0; i < validCount; i++) {
        free(validWords[i]);
    }
    free(validWords);
    for (int group = 0; group < MAX_GROUP; group++) {
        for (int j = 0; j < wordListSizes[group]; j++) {
            free(wordLists[group][j]);
        }
        free(wordLists[group]);
        free(guessed[group]);
    }
    free(wordLists);
    free(guessed);
    free(wordListSizes);
    free(letters);
    return 0;
}
