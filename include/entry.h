#ifndef ENTRY_H
#define ENTRY_H

typedef struct Entry {
    int id, rCount, wCount, readcnt;
    double time;
    sem_t mutex, wrt;
} Entry;

typedef Entry* EntryPtr;

#endif