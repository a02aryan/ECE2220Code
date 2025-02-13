#include <stdio.h>     
#include <stdlib.h>   
#include <time.h>      
#include <stdbool.h>   // For bool, true, false

// Prints the 5x5 board with columns labeled T, I, G, E, R.
void printBoard(char Tarray[], char Iarray[], char Garray[], char Earray[], char Rarray[], int SIZE) {
    printf("\n   T      I      G      E      R\n");      // Header row
    printf("------------------------------------\n");
    for (int i = 0; i < SIZE; i++) {
        // Check and print T column
        if (Tarray[i] == 'X') {
            printf("|  X   |");
        } else {
            printf("|  %2d  |", Tarray[i]);
        }

        // Check and print I column
        if (Iarray[i] == 'X') {
            printf("  X   |");
        } else {
            printf("  %2d  |", Iarray[i]);
        }

        // Check and print G column (account for 'F')
        if (Garray[i] == 'F') {
            printf(" FREE |");
        } else if (Garray[i] == 'X') {
            printf("  X   |");
        } else {
            printf("  %2d  |", Garray[i]);
        }

        // Check and print E column
        if (Earray[i] == 'X') {
            printf("  X   |");
        } else {
            printf("  %2d  |", Earray[i]);
        }

        // Check and print R column
        if (Rarray[i] == 'X') {
            printf("  X   |\n");
        } else {
            printf("  %2d  |\n", Rarray[i]);
        }

        // Print separator between rows
        if (i < SIZE - 1){
            printf("|------|------|------|------|------|\n");
        }
    }
    printf("------------------------------------\n");
}

// Checks if randomNum is not already in used[]
bool isUnique(int used[], int randomNum, int count) {
    for (int i = 0; i < count; i++) {
        if (used[i] == randomNum) {
            return false;// Duplicate found
        }
    }
    return true;// It's unique
}

// Generates a random number in the range
int generateRandomNum(int used[], int offset, int count, int maxRange, int size2) {
    int randomNum;
    do {
        // draws a random number
        randomNum = (rand() % maxRange) + offset;
    } while (!isUnique(used, randomNum, count) || randomNum == 88);

    // Store the random number in the first empty slot
    for (int i = 0; i < size2; i++) {
        if (used[i] == 0) {
            used[i] = randomNum;
            break;
        }
    }
    // Also store at index 'count'
    used[count] = randomNum;

    return randomNum;
}

// Marks the array element as 'X' if it matches drawNum
void marked(char array[], int drawNum){
    if (drawNum == array[0]) {
        array[0] = 'X';
    } else if (drawNum == array[1]) {
        array[1] = 'X';
    } else if (drawNum == array[2]) {
        array[2] = 'X';
    } else if (drawNum == array[3]) {
        array[3] = 'X';
    } else if (drawNum == array[4]) {
        array[4] = 'X';
    }
}

// Checks for a win in columns, rows, and diagonals
int checkWin(char Tarray[], char Iarray[], char Garray[], char Earray[], char Rarray[], int SIZE, int game_played) {
    int wincont = 0;

    // Check T column for full 'X'
    for (int i = 0; i < SIZE; i++) {
        if (Tarray[i] == 'X') {
            wincont++;
        }
    }
    if (wincont == SIZE) {
        printf("You win!\n");
        return 1;
    }

    // Check I column for full 'X'
    wincont = 0;
    for (int i = 0; i < SIZE; i++) {
        if (Iarray[i] == 'X') {
            wincont++;
        }
    }
    if (wincont == SIZE) {
        printf("You win!\n");
        return 1;
    }

    // Check G column for full 'X', middle is 'Free' visually but treated as 'X'
    wincont = 0;
    Garray[2] = 'X';  // Temporarily treat the free cell as marked
    for (int i = 0; i < SIZE; i++) {
        if (Garray[i] == 'X') {
            wincont++;
        }
    }
    if (wincont == SIZE) {
        printf("You win!\n");
        return 1;
    } else {
        Garray[2] = 'F'; // Restore the 'F'
    }

    // Check E column
    wincont = 0;
    for (int i = 0; i < SIZE; i++) {
        if (Earray[i] == 'X') {
            wincont++;
        }
    }
    if (wincont == SIZE) {
        printf("You win!\n");
        return 1;
    }

    // Check R column
    wincont = 0;
    for (int i = 0; i < SIZE; i++) {
        if (Rarray[i] == 'X') {
            wincont++;
        }
    }
    if (wincont == SIZE) {
        printf("You win!!!\n");
        return 1;
    }

    // Check rows individually (indices line up across arrays)
    if (Tarray[0] == 'X' && Iarray[0] == 'X' && Garray[0] == 'X' && Earray[0] == 'X' && Rarray[0] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[1] == 'X' && Iarray[1] == 'X' && Garray[1] == 'X' && Earray[1] == 'X' && Rarray[1] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[2] == 'X' && Iarray[2] == 'X' && Garray[2] == 'F' && Earray[2] == 'X' && Rarray[2] == 'X') {
        // Middle of G is 'Free' but is treat as 'X' logically
        printf("You win!\n");
        return 1;
    } else if (Tarray[3] == 'X' && Iarray[3] == 'X' && Garray[3] == 'X' && Earray[3] == 'X' && Rarray[3] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[4] == 'X' && Iarray[4] == 'X' && Garray[4] == 'X' && Earray[4] == 'X' && Rarray[4] == 'X') {
        printf("You win!\n");
        return 1;
    }

    // Check diagonals
    if (Tarray[0] == 'X' && Iarray[1] == 'X' && Garray[2] == 'F' && Earray[3] == 'X' && Rarray[4] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[4] == 'X' && Iarray[3] == 'X' && Garray[2] == 'F' && Earray[1] == 'X' && Rarray[0] == 'X') {
        printf("You win!\n");
        return 1;
    }

    return 0;  // No win found
}

int main() {
    const int SIZE = 5;           
    char Tarray[SIZE];            
    char Iarray[SIZE];            
    char Garray[SIZE];            
    char Earray[SIZE];            
    char Rarray[SIZE];         
    int quit = 0;  
    int game_played = 0;// Track how many games have been played
    int count = 0;                
    int moves = 0;// Counts  moves  made
    char input; // For user input to end/draw a number
    char Wininput; // For user response after a win
    int used[25] = {0};// Stores used numbers for board generation
    int drawGen[99] = {0};// Stores used numbers for draws
    int count2 = 0;// Tracks usage in 'drawGen[]'
    int counter = 1;// Simple counter to label each draw

    srand(time(NULL));// Seed the random generator

    // Generate the initial board
    for (int i = 0; i < SIZE; i++) {
        // Fill T column with 1-20
        Tarray[i] = generateRandomNum(used, 1, count, 20, 25);
        count++;

        // Fill I column with 21-40
        Iarray[i] = generateRandomNum(used, 21, count, 20, 25);
        count++;

        // Fill G column with 41-60 (middle cell is 'Free')
        if (i == 2) {
            Garray[i] = 'F';// The free space
        } else {
            Garray[i] = generateRandomNum(used, 41, count, 20, 25);
            count++;
        }

        // Fill E column with 61-80
        Earray[i] = generateRandomNum(used, 61, count, 20, 25);
        count++;

        // Fill R column with 81-99 
        Rarray[i] = generateRandomNum(used, 81, count, 19, 25);
        count++;
    }

    // Print the initial board
    printBoard(Tarray, Iarray, Garray, Earray, Rarray, SIZE);
    
    // Main game loop
    while (quit == 0) {
        printf("Press Enter to generate a square (or 'q' to quit): \n");
        input = getchar(); // Read user input

        // If 'q', end the game
        if (input == 'q' || input == 'Q') { 
            printf("Game over. Total Games: %d\n", game_played);
            quit = 1;
        }

        // Generate a random draw from 1-99
        int drawNum = generateRandomNum(drawGen, 1, count2, 99, 99);
        count2++;

        // Decide which column the number belongs to, then mark it
        if (quit == 0){
            if (drawNum <= 20){
                printf("%d: T-%d\n", counter, drawNum);
                marked(Tarray, drawNum);
            } else if (drawNum <= 40) {
                printf("%d: I-%d\n", counter, drawNum);
                marked(Iarray, drawNum);
            } else if (drawNum <= 60) {
                printf("%d: G-%d\n", counter, drawNum);
                marked(Garray, drawNum);
            } else if (drawNum <= 80) {
                printf("%d: E-%d\n", counter, drawNum);
                marked(Earray, drawNum);
            } else {
                printf("%d: R-%d\n", counter, drawNum);
                marked(Rarray, drawNum);
            }
        }
        moves++;    
        counter++;  

        // Print the board after the draw
        if (quit == 0){
            printBoard(Tarray, Iarray, Garray, Earray, Rarray, SIZE);
        }
        // Check if there's a winner
        if (checkWin(Tarray, Iarray, Garray, Earray, Rarray, SIZE, game_played) == 1) {
            printf("Total moves: %d\n", moves);
            printf("Press anything other then 'q' to to play again! (or 'q' to quit): ");
            Wininput = getchar();   // Wait for user decision

            // If user pressed 'q', end
            if (Wininput == 'q' || Wininput == 'Q') {
                game_played++;
                printf("Game over. Total Games: %d\n", game_played);
                quit = 1;
            } else {
                // Reset for a new game
                game_played++;
                count = 0;
                count2 = 0;
                moves = 0;
                counter = 1;

                // Clear used arrays
                for (int k = 0; k < 25; k++) {
                    used[k] = 0;
                }

                // Generate a new board
                for (int i = 0; i < SIZE; i++) {
                    Tarray[i] = generateRandomNum(used, 1, count, 20,25);
                    count++;
                    Iarray[i] = generateRandomNum(used, 21, count, 20,25);
                    count++;
                    if (i == 2) {
                        Garray[i] = 'F';
                    } else {
                        Garray[i] = generateRandomNum(used, 41, count, 20,25);
                        count++;
                    }
                    Earray[i] = generateRandomNum(used, 61, count, 20,25);
                    count++;
                    Rarray[i] = generateRandomNum(used, 81, count, 19,25);
                    count++;
                }
                if (quit == 0) {
                    printBoard(Tarray, Iarray, Garray, Earray, Rarray, SIZE);
                }
            }
        }
    }

    return 0;
}
