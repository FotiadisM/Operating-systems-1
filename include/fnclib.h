#ifndef FNCLIB_H
#define FNCLIB_H

#include "entry.h"

char readerOrWriter(int* curWriters, int* curReaders, int ratio);
/* Determines if the procces will be a reader or a writer */

void processAtWork(char isReader, EntryPtr mEntries, int entries);
/* Function that all child processes execute */

double randomExponential(double lambda);
/*Calculates radnom Expontetial time*/

#endif