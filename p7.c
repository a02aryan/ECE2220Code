/*
Name: Aryan Patel
CUID: ASP6
Course: ECE2220
Semester: Spring 2025
Assignment number: Project 6
Purpose: Grow three plants in separate terminals, handle water/feed signals, and show their status.
Assumptions: None
Known Bugs: None
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TERMINALS 4
#define MAX_SCAN 32

static volatile int fed = 0; /// Shared variable for fertilizer
static volatile int watered = 0;// Shared variable for water

void handleFed(int sig) {// Signal handler for fertilizer
    fed += 10000;
}

void handleWater(int sig) {// Signal handler for water
    watered += 1000;
}

void findterminals(FILE *ptrTerms[MAX_TERMINALS]) {// Find available terminal files
    char path[32];// Path to terminal files
    int count = 0;// Count of available terminals

    for (int i = 0; i < MAX_SCAN && count < MAX_TERMINALS; ++i) {// Scan for terminal files
        sprintf(path, "/dev/pts/%d", i);
        FILE *ptrCheck = fopen(path, "r");// Check if terminal file exists

        if (ptrCheck) {
            fclose(ptrCheck);
            ptrTerms[count] = fopen(path, "w");// Open terminal file for writing

            if (!ptrTerms[count]) {// Check if terminal file opened successfully
                exit(1);
            }

            ++count;
        }
    }

    if (count < MAX_TERMINALS) {
        exit(1);
    }
}

void plantInit(int indx, FILE *ptrterminal) {// Initialize plant in terminal
    fprintf(ptrterminal, "\n          \\|/\n");
    fprintf(ptrterminal, "         --%d--\n", indx);
    fprintf(ptrterminal, "          | |\n");
    fprintf(ptrterminal, "-------------------------\n");
}

void childProcess(int plant_indx, FILE *ptrterminal) {
    int fertilizer = 10000;// Initial fertilizer in mg
    int water = 1000;// Initial water in mL
    int height = 0;
    int dead = 0;
    int dayCount = 0;
    int day = 1;
    int LowWater = 0;
    int LowFert = 0;
    int addFed = 0;
    int addWatered = 0;

    srand((unsigned)(clock() ^ getpid()));// Seed random number generator

    signal(SIGUSR1, handleFed);// Signal handler for fertilizer
    signal(SIGUSR2, handleWater);// Signal handler for water

    plantInit(plant_indx, ptrterminal);// Initialize plant in terminal
    sleep(1);

    while (1) {
        sleep(1);

        fertilizer = fertilizer + fed;// Add fertilizer
        water = water + watered;// Add water

        if (fed > 0) addFed = 1;// Set flag for fertilizer
        if (watered > 0) addWatered = 1;// Set flag for water

        fed = 0;
        watered = 0;

        if (addFed || addWatered) {// Print status if fertilizer or water added
            fprintf(ptrterminal, "Plant %d: %d - %d mg of fertilizer left, %d mL of water left\n", plant_indx, getpid(), fertilizer, water);
            addFed = 0;// Reset flags
            addWatered = 0;// Reset flags
        }

        int subFert = rand() % 2001 + 1000;// Random fertilizer usage between 1000 and 3000 mg
        int subWater = rand() % 201 + 100;// Random water usage between 100 and 300 mL

        fertilizer = fertilizer - subFert;// Subtract fertilizer used
        water = water - subWater;// Subtract water used


        if (fertilizer < 0) fertilizer = 0;// Ensure fertilizer doesn't go negative
        if (water < 0) water = 0;// Ensure water doesn't go negative

        if (fertilizer < 5000 && !LowFert) {// Check if fertilizer is low
            fprintf(ptrterminal, "Plant %d almost out of fertilizer\n", plant_indx);
            LowFert = 1;// Set flag for low fertilizer
        }
        if (fertilizer >= 5000){
            LowFert = 0;
        }

        if (water < 500 && !LowWater) {//
            fprintf(ptrterminal, "Plant %d almost out of water\n", plant_indx);
            LowWater = 1;
        }
        if (water >= 500) {
            LowWater = 0;// Reset flag for low water
        }

        if (fertilizer == 0 || water == 0) {// Check if plant is dead
            dead++;// Increment dead counter
            if (dead >= 5) {// Check if plant has died
                fprintf(ptrterminal, "Plant %d has died.\n", plant_indx);
                exit(0);// Exit if plant is dead
            }
        } else {
            dead = 0;
        }

        if (++dayCount == 5) {// Check if 5 days have passed
            fprintf(ptrterminal, "\nDay %d\n", day++);
            fprintf(ptrterminal, "           \\|/\n");
            fprintf(ptrterminal, "          --%d--\n", plant_indx);
            fprintf(ptrterminal, "           | |\n");
            for (int i = 0; i < height; ++i) {
                fprintf(ptrterminal, "           | |\n");
            }
            fprintf(ptrterminal, "-------------------------\n");
            height++;//
            dayCount = 0;// Reset day count
        }

        if (height == 10) {// Check if plant is ready to sell
            fprintf(ptrterminal, "Plant %d is ready to be sold!\n", plant_indx);
            exit(0);
        }
    }
}

void makePlant(FILE *ptrTerms[MAX_TERMINALS], pid_t pids[3]) {// Spawn plant processes
    for (int i = 1; i <= 3; ++i) {// Create plant processes
        pid_t pid = fork();// Fork a new process

        if (pid < 0) {
            exit(1);
        }

        if (pid == 0) {// Child process
            childProcess(i, ptrTerms[i]);
            exit(0);
        }

        pids[i - 1] = pid;// Store process ID
    }
}

void plantloop(pid_t pids[3]) {// Gardener loop for user commands
    char buf[64];// Buffer for user input
    char cmd[16];// Command buffer
    int n;

    printf("Enter command (f<n>, w<n>, s, q): ");// Prompt for command

    while (fgets(buf, sizeof buf, stdin)) {// Read user input
        sscanf(buf, "%15s%d", cmd, &n);// Parse command and number

        if (strcmp(cmd, "s") == 0) {// Status command
            for (int i = 0; i < 3; i++) {// Check status of each plant
                int status;
                pid_t result = waitpid(pids[i], &status, WNOHANG);// Check if process is still running
                int alive = (result == 0);// Check if process is alive
            
                if (alive) {
                    printf("Plant %d: %d Alive\n", i + 1, pids[i]);// Print alive status
                } else {
                    printf("Plant %d: %d Dead\n", i + 1, pids[i]);// Print dead status
                }
            }
            
        } else if (strcmp(cmd, "q") == 0) {// Quit command
            for (int i = 0; i < 3; i++) {
                kill(pids[i], SIGKILL);
            }

            for (int i = 0; i < 3; i++) {// Wait for child processes to finish
                wait(NULL);
            }

            return;
        } else if (strcmp(buf, "f1\n") == 0) {// Fertilize command for plant 1
            kill(pids[0], SIGUSR1);
        } else if (strcmp(buf, "f2\n") == 0) {// Fertilize command for plant 2
            kill(pids[1], SIGUSR1);
        } else if (strcmp(buf, "f3\n") == 0) {// Fertilize command for plant 3
            kill(pids[2], SIGUSR1);
        } else if (strcmp(buf, "w1\n") == 0) {// Water command for plant 1
            kill(pids[0], SIGUSR2);
        } else if (strcmp(buf, "w2\n") == 0) {// Water command for plant 2
            kill(pids[1], SIGUSR2);
        } else if (strcmp(buf, "w3\n") == 0) {// Water command for plant 3
            kill(pids[2], SIGUSR2);
        } else {
            printf("Usage: f<n>, w<n>, s, q  (n=1..3)\n");// Invalid command
        }

        printf("Enter command (f<n>, w<n>, s, q): ");// Prompt for command again
    }
}

void startTime() {
    time_t now = time(NULL);
    char time[64];// Timestamp string

    strftime(time, sizeof time, "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("Program started at: %s\n", time);
}

int main(void) {
    FILE *ptrTerms[MAX_TERMINALS];// Array of terminal file pointers
    pid_t pids[3];// Array of process IDs

    findterminals(ptrTerms);// Find available terminals
    startTime();// Print start time
    makePlant(ptrTerms, pids);// Spawn plant processes
    plantloop(pids);// Start gardener loop

    return 0;
}