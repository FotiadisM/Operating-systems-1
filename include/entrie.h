#ifndef ENTRIE_H
#define ENTRIE_H

typedef struct Entrie {
    int id, rCount, wCount, readcnt;
    double time;
    sem_t mutex, wrt;
} Entrie;

typedef Entrie* EntriePtr;

#endif