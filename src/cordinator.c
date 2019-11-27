#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

#include "../include/entrie.h"
#include "../include/fnclib.h"

#define SEM_NAME "pSemaphore"

int main(int argc, char* argv[])
{
    char isReader;
    int shmID, peers, entries, readers, writers;
    pid_t pid;
    key_t key;
    EntriePtr mEntries;

    srand(time(NULL));

    if(argc != 5) {
        printf("Usage: ./bin/runner \"Number of childs\" \"Number of entries\" \"Number of readers\" \"Number of writers\"\n");
        return 0;
    }
    peers = atoi(argv[1]); entries = atoi(argv[2]); readers = atoi(argv[3]); writers = atoi(argv[4]);

    key = ftok("./build/cordinator.c", rand());
    shmID = shmget(key, entries*sizeof(Entrie), IPC_CREAT | 0666);
    if(shmID == -1) {
        perror("shmget() failed");
        return -1;
    }
    mEntries = (EntriePtr) shmat(shmID, NULL, 0);
    if(!mEntries) {
        perror("shmat() failed");
        return -1;
    }
    printf("Entries at start\n");
    for(int i=0; i < entries; i++) {
        mEntries[i].id = 1;
        sem_init(&mEntries[i].sem, 1, 1);
        printf("%d ", mEntries[i].id);
    }
    printf("\n");


    for(int i=0; i < peers; i++) {
        isReader = readerOrWriter(&readers, &writers);
        pid = fork();

        if(pid == -1) {
            perror("fork() failed");
            return -1;
        }
        else if(pid == 0) {
            printf("Process created: pid = %d\n", getpid());
            for(int i=0; i < 4; i++) {
                processAtWork(isReader, mEntries, entries);
                // sleep(1);
            }
            exit(0);
        }
    }

    for(int i=0; i < peers; i++) {
        pid = wait(NULL);
        printf("Process with pid = %d exited\n", pid);
    }
    printf("\nEntries at the end\n");
    for(int i=0; i < entries; i++) {
        printf("%d ", mEntries[i].id);
        sem_destroy(&mEntries[i].sem);
    }
    printf("\n");

    shmdt(mEntries);
    shmctl(shmID, IPC_RMID, NULL);

    return 0;
}