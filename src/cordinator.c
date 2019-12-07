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
#include "../include/defines.h"

int main(int argc, char* argv[])
{
    int shmID, peers, entries, ratio;
    pid_t pid;
    key_t key;
    EntriePtr mEntries;

    srand(time(NULL));

    if(argc != 4) {
        printf("Usage:\n./bin/runner \"Peers\" \"Entries\" \"Readers/Writers ratio\"\n");
        return 0;
    }

    peers = atoi(argv[1]); entries = atoi(argv[2]); ratio = atoi(argv[3]);

    if(peers == 0) {
        printf("The program need at least one peer to run\n");
        return 0;
    }
    else if(entries == 0) {
        printf("The program need at least one entrie to run");
        return 0;
    }

    key = ftok(KEY_STRING, rand());
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
    printf("Printing Entries' initial values:\n");
    for(int i=0; i < entries; i++) {
        mEntries[i].id = 1;
        mEntries[i].rCount = 0; mEntries[i].wCount = 0;
        mEntries[i].time = 0; mEntries[i].readcnt = 0;
        sem_init(&mEntries[i].mutex, 1, 1); sem_init(&mEntries[i].wrt, 1, 1);
        printf("%d ", mEntries[i].id);
    }
    printf("\n\n");


    for(int i=0; i < peers; i++) {
        pid = fork();

        if(pid == -1) {
            perror("fork() failed");
            return -1;
        }
        else if(pid == 0) {
            int curWriters = 0, curReaders = 0;

            srand((int)getpid());
            printf("Process created: pid = %d\n", getpid());
            for(int i=0; i < NUMBER_OF_ITERATIONS; i++) {
                processAtWork(readerOrWriter(&curWriters, &curReaders, ratio), mEntries, entries);
            }
            exit(0);
        }
    }

    for(int i=0; i < peers; i++) {
        pid = wait(NULL);
        printf("Process with pid = %d exited\n", pid);
    }

    printf("\n\n|Entry number|Value|Times read|Times written|Average time|\n");
    for(int i=0; i < entries; i++) {
        double Time = 0.0;

        if(mEntries[i].wCount) {
            Time = mEntries[i].time/mEntries[i].wCount;
        }

        printf("|%12d|%5d|%10d|%13d|%12f|\n", i, mEntries[i].id, mEntries[i].rCount, mEntries[i].wCount, Time);
        sem_destroy(&mEntries[i].mutex);
        sem_destroy(&mEntries[i].wrt);
    }
    printf("|--------------------------------------------------------|\n\n");

    shmdt(mEntries);
    shmctl(shmID, IPC_RMID, NULL);

    return 0;
}