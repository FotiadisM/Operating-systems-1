#ifndef FNCLIB_H
#define FNCLIB_H

#include "entrie.h"

char readerOrWriter(int *readers, int* writers);

void processAtWork(char isReader, EntriePtr mEntries);

#endif