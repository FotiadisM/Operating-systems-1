#ifndef ENTRIE_H
#define ENTRIE_H

typedef struct Entrie {
    int id;
    sem_t sem;
} Entrie;

typedef Entrie* EntriePtr;

#endif