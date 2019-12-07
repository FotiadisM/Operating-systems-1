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

#include "../include/entry.h"
#include "../include/fnclib.h"
#include "../include/defines.h"

int cordinator(int peers, int entries, int ratio)
{
    int shmID, totatReads = 0 , totalWrites = 0;
    pid_t pid;
    key_t key;
    EntryPtr mEntries;

    srand(time(NULL));

    key = ftok(KEY_STRING, rand());
    shmID = shmget(key, entries*sizeof(Entry), IPC_CREAT | 0666);
    if(shmID == -1) {
        perror("shmget() failed");
        return -1;
    }

    mEntries = (EntryPtr) shmat(shmID, NULL, 0);
    if(!mEntries) {
        perror("shmat() failed");
        return -1;
    }

    for(int i=0; i < entries; i++) {
        mEntries[i].id = 1;
        mEntries[i].rCount = 0; mEntries[i].wCount = 0;
        mEntries[i].time = 0; mEntries[i].readcnt = 0;
        sem_init(&mEntries[i].mutex, 1, 1); sem_init(&mEntries[i].wrt, 1, 1);
    }
    printf("\n");


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

        totatReads += mEntries[i].rCount;
        totalWrites += mEntries[i].wCount;
        Time = mEntries[i].time/(mEntries[i].wCount + mEntries[i].rCount);

        printf("|%12d|%5d|%10d|%13d|%12f|\n", i, mEntries[i].id, mEntries[i].rCount, mEntries[i].wCount, Time);
        sem_destroy(&mEntries[i].mutex);
        sem_destroy(&mEntries[i].wrt);
    }
    printf("|--------------------------------------------------------|\n\n");
    printf("Total Reads: %d\nTotal Writes: %d\n\n", totatReads, totalWrites);

    shmdt(mEntries);
    shmctl(shmID, IPC_RMID, NULL);

    return 0;
}