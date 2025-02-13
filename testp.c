#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateBoard(int board[5][5]);
void printBoard(int board[5][5], int marked[5][5]);

int main() {
    int board[5][5];
    int marked[5][5] = {0}; // Keep track of marked squares
    int counter = 0;
    char input;

    srand(time(NULL));

    generateBoard(board);
    marked[2][2] = 1; // Mark "Free" square

    printf("Welcome to the TIGER game!\n\n");
    printBoard(board, marked);

    while (1) {
        printf("Press Enter to generate a square (or 'q' to quit): ");
        input = getchar();
        if (input == 'q' || input == 'Q') {
            printf("Game over. Total moves: %d\n", counter);
            break;
        }

        int col = rand() % 5;
        int row = rand() % 5;
        counter++;

        printf("Move %d: Marking [%d][%d]\n", counter, row, col);
        marked[row][col] = 1;

        printBoard(board, marked);
    }

    return 0;
}

void generateBoard(int board[5][5]) {
    int start = 1;
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 5; row++) {
            if (col == 2 && row == 2) {
                board[row][col] = 0; // Free square
            } else {
                board[row][col] = start++;
            }
        }
    }
}

void printBoard(int board[5][5], int marked[5][5]) {
    printf("   T      I      G      E      R\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == 0) {
                printf("|  %s  ", marked[i][j] ? " X " : "Free");
            } else {
                printf("|  %3s ", marked[i][j] ? " X " : "");
                printf("%-2d", board[i][j]);
            }
        }
        printf("|\n-----------------------------------\n");
    }
}
