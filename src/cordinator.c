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
    sem_t* sem;
    EntriePtr mEntries;

    srand(time(NULL));

    if(argc != 5) {
        printf("Usage: ./bin/runner \"Number of childs\" \"Number of entries\" \"Number of readers\" \"Number of writers\"\n");
        return 0;
    }
    peers = atoi(argv[1]); entries = atoi(argv[2]); readers = atoi(argv[3]); writers = atoi(argv[4]);

    sem = sem_open (SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    sem_close(sem);


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
    for(int i=0; i < entries; i++) {
        mEntries[i].id = 0;
    }


    for(int i=0; i < peers; i++) {
        isReader = readerOrWriter(&readers, &writers);
        pid = fork();

        if(pid == -1) {
            perror("fork() failed");
            return -1;
        }
        else if(pid == 0) {
            printf("Child process with pid: %d was created\n", getpid());
            sem = sem_open (SEM_NAME, 0);
            sem_wait(sem);
            processAtWork(isReader, mEntries);
            sem_post(sem);
            sem_close(sem);
            exit(0);
        }
    }

    for(int i=0; i < peers; i++) {
        pid = wait(NULL);
        printf("Child process with pid: %d exited\n", pid);
    }

    sem_unlink(SEM_NAME);
    shmdt(mEntries);
    shmctl(shmID, IPC_RMID, NULL);

    return 0;
}