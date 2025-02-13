#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <stdbool.h>

int main()
{
    const int ROW = 5;
    const int COL = 5;
    int cont = 1;
    int game_played = 0;

    char array[ROW][COL] = {};
    bool used [25] = {false};
    char free[4] = {"Free"};

    
    srand(time(NULL));
    
    for (int col = 0; col < COL; col++) {
        for (int row = 0; row < ROW; row++) {
            if (col == 2 && row == 2) {
                array[row][col] = 0;
            } else {
                do {
                    int randomnum = ((rand() % 20) );
                    array[row][col] = (col*20 + randomnum);
                } while (used[array[row][col]]);
                used[array[row][col]] = true;

                if (array[row][col] >= 100) {
                    array[row][col] = 99;
                }
            }
        }
    }
    printf("   T      I      G      E      R\n");
    printf("------------------------------------\n");
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (array[i][j] < 10) {
                printf("|  %d   ", array[i][j]);
            } else if (array[i][j] > 80){
                printf("|  %d  |", array[i][j]);
            } 
            else if (array[i][j] == 0) {
                array[i][j] = 0;
            }
            else {
                printf("|  %d  ", array[i][j]);
            }
        }
        if (i < ROW - 1){
        printf("\n|------|------|------|------|------|\n");
        }
    }
    printf("\n------------------------------------\n");

//    while (cont == 1){
//        char input;
//         printf("Press ‘Enter’ to generate a Square (or ‘q’ to quit)...");
//         scanf("%s", input);
//        if (input == 'q') {
//            printf("Game over. Total games: %d\n", game_played);
//            break;
//        }
//    }
    return 0;
}

