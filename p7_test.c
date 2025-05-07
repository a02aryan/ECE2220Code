#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TERMINALS 20
#define NUM_PLANTS 3

typedef void (*fptr)();

typedef struct {
    int num;
    int fertilizer;
    int water;
    int height;
    int daysWithoutResources;
    FILE *terminal;
} PlantState;

typedef struct {
    pid_t pid;
    int alive;
} PlantInfo;

typedef struct {
    FILE *files[MAX_TERMINALS];
    char paths[MAX_TERMINALS][32];
    int count;
} TerminalInfo;

// We need this shared variable for the signal handlers
PlantState *signalState = NULL;

// Signal handlers
void signalHandler(int signum) {
    if (!signalState) return;
    
    if (signum == SIGUSR1) {
        signalState->fertilizer += 10000; // Add 10g
        fprintf(signalState->terminal, "Plant %d: %d -- %d mg of fertilizer left\n%d mL of water left\n", 
                signalState->num, getpid(), signalState->fertilizer, signalState->water);
    } else if (signum == SIGUSR2) {
        signalState->water += 1000; // Add 1L
        fprintf(signalState->terminal, "Plant %d: %d -- %d mg of fertilizer left\n%d mL of water left\n", 
                signalState->num, getpid(), signalState->fertilizer, signalState->water);
    }
}

// Plant process function
void plantProcess(int num, TerminalInfo *terminals) {
    PlantState state;
    
    // Initialize plant state
    state.num = num;
    state.fertilizer = 10000; // 10g in mg
    state.water = 1000; // 1L in mL
    state.height = 0;
    state.daysWithoutResources = 0;
    state.terminal = fopen(terminals->paths[num], "w");
    
    if (state.terminal == NULL) {
        exit(1);
    }
    
    // We need to use this shared variable for the signal handlers
    signalState = &state;
    
    // Set up signal handlers
    signal(SIGUSR1, (fptr)signalHandler);
    signal(SIGUSR2, (fptr)signalHandler);
    
    // Initialize plant
    srand(time(NULL) ^ getpid());
    
    // Initial plant drawing - clear screen first
    fprintf(state.terminal, "\033[H\033[J");
    fprintf(state.terminal, "\\|/\n");
    fprintf(state.terminal, "--%d--\n", state.num);
    fprintf(state.terminal, "----------------------\n");
    fprintf(state.terminal, "Fertilizer: %d mg\n", state.fertilizer);
    fprintf(state.terminal, "Water: %d mL\n", state.water);
    
    // Track the last "day" time
    time_t lastGrowthTime = time(NULL);
    
    // Main plant loop
    while (1) {
        // Reduce resources
        int fertilizerReduction = (rand() % 2001) + 1000; // 1000-3000 mg
        int waterReduction = (rand() % 201) + 100;       // 100-300 mL
        
        state.fertilizer -= fertilizerReduction;
        state.water -= waterReduction;
        
        if (state.fertilizer < 0) state.fertilizer = 0;
        if (state.water < 0) state.water = 0;
        
        // Print plant status
        fprintf(state.terminal, "\033[H\033[J"); // Clear screen
        fprintf(state.terminal, "\\|/\n");
        fprintf(state.terminal, "--%d--\n", state.num);
        
        for (int i = 0; i < state.height; i++) {
            fprintf(state.terminal, "| |\n");
        }
        
        fprintf(state.terminal, "----------------------\n");
        fprintf(state.terminal, "Fertilizer: %d mg\n", state.fertilizer);
        fprintf(state.terminal, "Water: %d mL\n", state.water);
        
        // Check if resources are low
        if (state.fertilizer < 5000) {
            fprintf(state.terminal, "Plant %d almost out of fertilizer\n", num);
        }
        
        if (state.water < 500) {
            fprintf(state.terminal, "Plant %d almost out of water\n", num);
        }
        
        // Check for growth (every 5 seconds)
        time_t currentTime = time(NULL);
        if (currentTime - lastGrowthTime >= 5) {
            state.height++;
            lastGrowthTime = currentTime;
            
            fprintf(state.terminal, "Day %d\n", state.height);
        }
        
        // Check for death or readiness for sale
        if (state.fertilizer == 0 || state.water == 0) {
            state.daysWithoutResources++;
            
            if (state.daysWithoutResources >= 5) {
                fprintf(state.terminal, "Plant %d has died!\n", num);
                fclose(state.terminal);
                exit(1);
            }
        } else {
            state.daysWithoutResources = 0;
        }
        
        // Check if plant is ready to sell
        if (state.height >= 10) {
            fprintf(state.terminal, "Plant %d is ready to be sold!\n", num);
            fclose(state.terminal);
            exit(0);
        }
        
        sleep(1);
    }
}

int main() {
    PlantInfo plants[NUM_PLANTS];
    TerminalInfo terminals;
    char cmd[10];
    int plantNum;
    
    // Initialize terminal info
    terminals.count = 0;
    
    // Initialize plants
    for (int i = 0; i < NUM_PLANTS; i++) {
        plants[i].pid = 0;
        plants[i].alive = 0;
    }
    
    // Find available terminals
    FILE *term;
    char path[32];
    
    for (int i = 0; i < MAX_TERMINALS; i++) {
        sprintf(path, "/dev/pts/%03d", i);
        term = fopen(path, "w");
        
        if (term != NULL) {
            terminals.files[terminals.count] = term;
            strcpy(terminals.paths[terminals.count], path);
            terminals.count++;
            
            if (terminals.count >= 4) {
                break; // Found enough terminals
            }
        }
    }
    
    if (terminals.count < 4) {
        printf("Error: Need at least 4 open terminal windows\n");
        exit(1);
    }
    
    // Print date and time
    time_t current_time;
    struct tm *time_info;
    
    time(&current_time);
    time_info = localtime(&current_time);
    
    printf("Program started: %s", asctime(time_info));
    
    // Create plant processes
    for (int i = 0; i < NUM_PLANTS; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            // Error on fork
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process (plant)
            plantProcess(i + 1, &terminals);
            exit(0);
        } else {
            // Parent process
            plants[i].pid = pid;
            plants[i].alive = 1;
            printf("Created plant %d with PID %d\n", i + 1, pid);
        }
    }
    
    // Main command loop
    while (1) {
        printf("Enter command (f<n>, w<n>, s, q): ");
        
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            break;
        }
        
        if (cmd[0] == 'f' && sscanf(cmd, "f%d", &plantNum) == 1) {
            // Feed command
            if (plantNum < 1 || plantNum > NUM_PLANTS) {
                printf("Invalid plant number\n");
            } else if (!plants[plantNum - 1].alive) {
                printf("Plant %d is not alive\n", plantNum);
            } else {
                // Send SIGUSR1 to add fertilizer
                kill(plants[plantNum - 1].pid, SIGUSR1);
                printf("Plant %d fed with fertilizer\n", plantNum);
            }
        } else if (cmd[0] == 'w' && sscanf(cmd, "w%d", &plantNum) == 1) {
            // Water command
            if (plantNum < 1 || plantNum > NUM_PLANTS) {
                printf("Invalid plant number\n");
            } else if (!plants[plantNum - 1].alive) {
                printf("Plant %d is not alive\n", plantNum);
            } else {
                // Send SIGUSR2 to add water
                kill(plants[plantNum - 1].pid, SIGUSR2);
                printf("Plant %d watered\n", plantNum);
            }
        } else if (cmd[0] == 's') {
            // Status command
            for (int i = 0; i < NUM_PLANTS; i++) {
                int status = 0;
                
                // Check if process is still running
                if (plants[i].alive) {
                    pid_t result = waitpid(plants[i].pid, &status, WNOHANG);
                    
                    if (result == 0) {
                        printf("Plant %d: %d Alive\n", i + 1, plants[i].pid);
                    } else {
                        printf("Plant %d: %d Dead\n", i + 1, plants[i].pid);
                        plants[i].alive = 0;
                    }
                } else {
                    printf("Plant %d: %d Dead\n", i + 1, plants[i].pid);
                }
            }
        } else if (cmd[0] == 'q') {
            printf("Exiting program\n");
            break;
        } else {
            printf("Invalid command\n");
        }
    }
    
    // Clean up by killing any remaining plants
    for (int i = 0; i < NUM_PLANTS; i++) {
        if (plants[i].alive) {
            kill(plants[i].pid, SIGKILL);
        }
    }
    
    // Close terminals
    for (int i = 0; i < terminals.count; i++) {
        if (terminals.files[i] != NULL) {
            fclose(terminals.files[i]);
        }
    }
    
    return 0;
}