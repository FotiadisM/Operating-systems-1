#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>

#include "../include/fnclib.h"
#include "../include/defines.h"

char readerOrWriter(int* curWriters, int* curReaders, int ratio)
{
    int isReader, maxReaders, maxWriters;

    maxWriters = NUMBER_OF_ITERATIONS/(ratio + 1);
    maxReaders = NUMBER_OF_ITERATIONS - maxWriters;

    isReader = rand()%2;
    if(!isReader) {
        if(*curWriters == maxWriters) {
            isReader = 1;
            (*curReaders)++;
        }
        else
            (*curWriters)++;
    }
    else {
        if(*curReaders == maxReaders) {
            isReader = 0;
            (*curWriters)++;
        }
        else
            (*curReaders)++;
    }

    return isReader;
}

double randomExponential(double lambda)
{
    double u;

    u = rand() / (RAND_MAX + 1.0);

    return -log(1-u) / lambda;
}

void processAtWork(char isReader, EntriePtr mEntries, int entries)
{
    int index = rand()%entries;
    clock_t start, end;

    // index = 9;

    if(isReader) {
        sem_wait(&mEntries[index].mutex);
        mEntries[index].readcnt++;
        if(mEntries[index].readcnt == 1) {
            sem_wait(&mEntries[index].wrt);
        }

        sem_post(&mEntries[index].mutex);

        printf("\tReading entry: %d, value = %d, pid:%d\n", index, mEntries[index].id, getpid());
        mEntries[index].rCount++;
        // mEntries[index].readingTime += expTime;
        sleep(randomExponential(LAMBDA));

        sem_wait(&mEntries[index].mutex);

        mEntries[index].readcnt--;
        if(mEntries[index].readcnt == 0) {
            sem_post(&mEntries[index].wrt);
        }
        sem_post(&mEntries[index].mutex);
    }
    else {
        start = clock();
        sem_wait(&mEntries[index].wrt);
        end = clock();

        printf("\tWriting on entry: %d, pid:%d\n", index, getpid());
        mEntries[index].id++;
        mEntries[index].wCount++;
        mEntries[index].time += ((double) (end - start)) / CLOCKS_PER_SEC;
        sleep(randomExponential(LAMBDA));

        sem_post(&mEntries[index].wrt);
    }
}