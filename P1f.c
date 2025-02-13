#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void printBoard(char Tarray[], char Iarray[], char Garray[], char Earray[], char Rarray[], int SIZE) {
    printf("\n   T      I      G      E      R\n");
    printf("------------------------------------\n");
    for (int i = 0; i < SIZE; i++) {
        if (Tarray[i] == 'X') {
            printf("|  X   |");
        } else {
            printf("|  %2d  |", Tarray[i]);
        }
        if (Iarray[i] == 'X') {
            printf("  X   |");
        } else {
            printf("  %2d  |", Iarray[i]);
        }
        if (Garray[i] == 'F') {
            printf(" FREE |");
        } else if (Garray[i] == 'X') {
            printf("  X   |");
        } else {
            printf("  %2d  |", Garray[i]);
        }
        if (Earray[i] == 'X') {
            printf("  X   |");
        } else {
            printf("  %2d  |", Earray[i]);
        }
        if (Rarray[i] == 'X') {
            printf("  X   |\n");
        } else {
            printf("  %2d  |\n", Rarray[i]);
        }
        if (i < SIZE - 1){
            printf("|------|------|------|------|------|\n");
        }
    }
    printf("------------------------------------\n");

}
bool isUnique(int used[], int randomNum, int count) {
    for (int i = 0; i < count; i++) {
        if (used[i] == randomNum) {
            return false;
        }
    }
    return true;
}
int generateRandomNum(int used[], int offset, int *count, int maxRange) {
    int randomNum;
    do {
        randomNum = (rand() % maxRange) + offset; 
    } while (!isUnique(used, randomNum, *count));
    used[*count] = randomNum;
    (*count)++; 
    return randomNum;
}
void marked(char array[],int drawNum){
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
int checkWin(char Tarray[], char Iarray[], char Garray[], char Earray[], char Rarray[], int SIZE, int game_played) {
    int wincont = 0;

    for (int i = 0; i < SIZE; i++) {
        if (Tarray[i] == 'X') {
            wincont++;
        }
    }
    if (wincont == SIZE) {
        printf("You win!\n");
        return 1;
    }
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
    wincont = 0;
    for (int i = 0; i < SIZE; i++) {
        Garray[2] = 'X';
        if (Garray[i] == 'X') {
            wincont++;
        }
    }
    if (wincont == SIZE) {
        printf("You win!\n");
        return 1;
    } else {
        Garray[2] = 'F';
    }
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

    if (Tarray[0] == 'X' && Iarray[0] == 'X' && Garray[0] == 'X' && Earray[0] == 'X' && Rarray[0] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[1] == 'X' && Iarray[1] == 'X' && Garray[1] == 'X' && Earray[1] == 'X' && Rarray[1] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[2] == 'X' && Iarray[2] == 'X' && Garray[2] == 'F' && Earray[2] == 'X' && Rarray[2] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[3] == 'X' && Iarray[3] == 'X' && Garray[3] == 'X' && Earray[3] == 'X' && Rarray[3] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[4] == 'X' && Iarray[4] == 'X' && Garray[4] == 'X' && Earray[4] == 'X' && Rarray[4] == 'X') {
        printf("You win!\n");
        return 1;
    }

    if (Tarray[0] == 'X' && Iarray[1] == 'X' && Garray[2] == 'F' && Earray[3] == 'X' && Rarray[4] == 'X') {
        printf("You win!\n");
        return 1;
    } else if (Tarray[4] == 'X' && Iarray[3] == 'X' && Garray[2] == 'F' && Earray[1] == 'X' && Rarray[0] == 'X') {
        printf("You win!\n");
        return 1;
    }

    return 0; 
}
int main() {
    const int SIZE = 5;
    char Tarray[SIZE];
    char Iarray[SIZE];
    char Garray[SIZE];
    char Earray[SIZE];
    char Rarray[SIZE];
    int used[25] = {0};
    int game_played = 0;
    int count = 0; 
    int gen[25] = {0};
    int count2 = 0;
    int wincont = 0;
    int moves = 0;
    char input;
    char input2;
    srand(time(NULL));

    for (int i = 0; i < SIZE; i++) {
        Tarray[i] = generateRandomNum(used, 1, &count, 20);
        Iarray[i] = generateRandomNum(used, 21, &count, 20);
        if (i == 2) {
            Garray[i] = 'F';
        } else {
            Garray[i] = generateRandomNum(used, 41, &count, 20);
        }
        Earray[i] = generateRandomNum(used, 61, &count, 20);
        Rarray[i] = generateRandomNum(used, 80, &count, 20);
    }

    printBoard(Tarray, Iarray, Garray, Earray, Rarray, SIZE);
    
    while (1){
        printf("Press Enter to generate a square (or 'q' to quit): ");
        input = getchar();

        if (input == 'q' || input == 'Q') {
            printf("Game over. Total Games: %d\n", game_played);
            break;
        }
        int drawNum = generateRandomNum(gen, 0, &count2,100);

        if (drawNum <= 20){
            printf("T-%d\n", drawNum);
            marked(Tarray,drawNum);
            moves++;
        } else if (drawNum <= 40) {
            printf("I-%d\n", drawNum);
            marked(Iarray,drawNum);
            moves++;
        } else if (drawNum <= 60) {
            printf("G-%d\n", drawNum);
            marked(Garray,drawNum);
            moves++;
        } else if (drawNum <= 80) {
            printf("E-%d\n", drawNum);
            marked(Earray,drawNum);
            moves++;
        } else {
            printf("R-%d\n", drawNum);
            marked(Rarray,drawNum);
            moves++;
        }
        
        printBoard(Tarray, Iarray, Garray, Earray, Rarray, SIZE);
        if (checkWin(Tarray, Iarray, Garray, Earray, Rarray, SIZE, game_played) == 1) {
            printf("Total moves: %d\n", moves);
            printf("Press anything other then 'q' to to play again! (or 'q' to quit): ");
            input2 = getchar();


        if (input2 == 'q' || input2 == 'Q') {
            printf("Game over. Total Games: %d\n", game_played);
            break;
        }
            input2 = getchar();
                game_played++;
                count2 = 0;
                count = 0;
                for (int i = 0; i < SIZE; i++) {
                    Tarray[i] = generateRandomNum(used, 1, &count, 20);
                    Iarray[i] = generateRandomNum(used, 21, &count, 20);
                    if (i == 2) {
                        Garray[i] = 'F';
                    } else {
                        Garray[i] = generateRandomNum(used, 41, &count, 20);
                    }
                    Earray[i] = generateRandomNum(used, 61, &count, 20);
                    Rarray[i] = generateRandomNum(used, 80, &count, 20);
                }
                printBoard(Tarray, Iarray, Garray, Earray, Rarray, SIZE);
            }

    }

    return 0;
}