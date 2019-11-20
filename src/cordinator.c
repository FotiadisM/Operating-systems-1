#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    pid_t pid = fork();

    if(pid < 0) {
        perror("Fork failed");
    }
    else if(pid == 0) {
        printf("My pid is: %d\n", pid);
        exit(0);
    }

    printf("My pid is: %d\n", pid);
    pid = wait(NULL);
    if(pid == -1) {
        perror("Wait failed");
    }

    return 0;
}