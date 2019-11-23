#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/entrie.h"

int main(int argc, char* argv[]) {

    int shmID;
    pid_t pid;
    key_t key;
    EntriePtr mEntries;

    if(argc != 5) {
        printf("Usage: ./build/runner \"Number of childs\" \"Number of entries\" \"Number of readers\" \"Number of writers\"\n");
        return 0;
    }

    key = ftok("./build/cordinator.c", 100);
    shmID = shmget(key, atoi(argv[2])*sizeof(Entrie), IPC_CREAT | 0666);
    mEntries = (EntriePtr) shmat(shmID, NULL, 0);
    for(int i=0; i < atoi(argv[2]); i++) {
        mEntries[i].id = 33;
    }

    for(int i=0; i < atoi(argv[1]); i++) {
        pid = fork();

        if(pid == -1) {
            perror("Fork failed");
            return 0;
        }
        else if(pid == 0) {
            for(int j=0; j < atoi(argv[2]); j++) {
                printf("skss %d\n", mEntries[j].id);
            }
            exit(0);
        }
    }

    shmdt(mEntries);
    shmctl(shmID, IPC_RMID, NULL);

    return 0;
}