#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#include "../include/fnclib.h"

char readerOrWriter(int *readers, int* writers)
{
    if(rand()%2) {
        if(*readers != 0) {
            (*readers)--;
            return 1;
        }
        (*writers)--;
        return 0;
    }
    if(*writers != 0) {
        (*writers)--;
        return 0;
    }
    (*readers)--;
    return 1;
}

void processAtWork(char isReader, EntriePtr mEntries, int entries)
{
    int index = rand()%entries;

    if(isReader) {
        sem_wait(&mEntries[index].sem);
        sem_post(&mEntries[index].sem);
        printf("\tReading entry: %d, value = %d, pid:%d\n", index, mEntries[index].id, getpid());
    }
    else {
        sem_wait(&mEntries[index].sem);
        printf("\tWriting on entry: %d, pid:%d\n", index, getpid());
        mEntries[index].id++;
        sem_post(&mEntries[index].sem);
    }

}