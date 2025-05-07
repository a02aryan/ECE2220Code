#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid1, pid2, pid3;
    pid1 = fork();
    if (pid1 == 0) {
        printf("I am number 1\n");
        exit(0);
    }
    sleep(1);
    pid2 = fork();
    if (pid2 == 0) {
        printf("I am number 2\n");
        exit(0);
    }
    sleep(1);
    pid3 = fork();
    if (pid3 == 0) {
        printf("I am number 3\n");
        exit(0);
    }
    waitpid(pid2, NULL, 0);
    printf("Go Tigers!\n");

    return 0;


}