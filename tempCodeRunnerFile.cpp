#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <stdbool.h>

int main()
{
    const int SIZE = 5;
    int cont = 1;
    int game_played = 0;

    char Tarray[SIZE] = {};
    char Iarray[SIZE] = {};
    char Garray[SIZE] = {};
    char Earray[SIZE] = {};
    char Rarray[SIZE] = {};

    srand(time(NULL));

    for (int i = 0; i < SIZE; i++)
    {
        Tarray[i] = (rand() % 20) + 1;
        Iarray[i] = (rand() % 20) + 16;
        Garray[i] = (rand() % 20) + 31;
        Earray[i] = (rand() % 20) + 46;
        Rarray[i] = (rand() % 20) + 61;
        Garray[2] = "f";
    }
    printf("   T      I      G      E      R\n");
    printf("------------------------------------\n");
    for (int i = 0; i < SIZE; i++) {
            printf("|  %2d  |", Tarray[i]);
            printf("  %2d  |", Iarray[i]);
            printf("  %2d  |", Garray[i]);
            printf("  %2d  |", Earray[i]);
            printf("  %2d  |", Rarray[i]);
            printf("\n|------|------|------|------|------|\n");
        }
    return 0;
}