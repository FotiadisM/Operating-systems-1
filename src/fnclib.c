#include <stdio.h>
#include <stdlib.h>
#include "../include/fnclib.h"

char readerOrWriter(int *readers, int* writers)
{
    if(rand()%2) {
        if(*readers) {
            (*readers)--;
            return 1;
        }
        (*writers)--;
        return 0;
    }
    if(*writers) {
        (*writers)--;
        return 0;
    }
    (*readers)--;
    return 1;
}

void processAtWork(char isReader, EntriePtr mEntries) {
    if(isReader) {
        printf("reader\n");
    }
    else {
        printf("writer\n");
    }
}