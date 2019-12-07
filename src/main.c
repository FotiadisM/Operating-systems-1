#include <stdio.h>
#include <stdlib.h>

#include "../include/cordinator.h"

int main(int argc, char* argv[])
{
    int peers, entries, ratio;

    if(argc != 4) {
        printf("Usage:\n./bin/runner \"Peers\" \"Entries\" \"Readers/Writers ratio\"\n");
        return 0;
    }

    peers = atoi(argv[1]); entries = atoi(argv[2]); ratio = atoi(argv[3]);

    if(peers == 0) {
        printf("The program need at least one peer to run\n");
        return 0;
    }
    else if(entries == 0) {
        printf("The program need at least one entrie to run");
        return 0;
    }

    if(cordinator(peers, entries, ratio) == -1) {
        perror("cordinator() failed");
        return -1;
    }

    return 0;
}