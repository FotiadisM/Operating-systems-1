#include <stdio.h>
#include <stdlib.h>
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

void processAtWork(char isReader, EntriePtr mEntries)
{
    // sem_wait(sem);

    if(isReader) {
        printf("reader: %d\n", isReader);
    }
    else {
        printf("writer: %d\n", isReader);
    }
}