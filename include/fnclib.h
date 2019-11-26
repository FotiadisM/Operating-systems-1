#ifndef FNCLIB_H
#define FNCLIB_H

#include "entrie.h"

char readerOrWriter(int *readers, int* writers);
/* Determines if the procces will be a reader or a writer */

void processAtWork(char isReader, EntriePtr mEntries, int entries);
/* Function that all child processes execute */
#endif