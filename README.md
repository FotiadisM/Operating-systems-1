## Compiling the program

`make`

## Runnign the program

`./bin/runner "Childs" "Entries" "Readers" "Writers"`

or with the default settings (./bin/runner 8 10 5 3):

`make run`

## Aditional Information

Το πρόγραμμα δευσμεύει χώρο στο shared memory ίσο με `atoi(argv[2])*sizeof(Entrie)`
ώστε ενα δημιουργήσει ενα πίνακα με Entries (`EntriePtr mEntries;`) όπου Entry μια δομή με τύπο:
```
typedef  struct  Entrie  {
	int id;
	sem_t sem;
}  Entrie;
typedef Entrie* EntriePtr;
```
Σε κάθε Entrie περιέχεται ενα semaphore που καθορίζει αν το Entrie είναι διαθέσιμο ή οχι και ενα id που γίνεται initialized με την τιμή 1.
Στην συνέχεια δημιουργούνται `atoi(argv[1])` child processes όπου στο καθένα αναθέτεται μια λειτουργία, Reader ή Writer (Η λειτουργία αυτή αναθέτεται τυχαία αρκεί να μην ξεπερνίεται ο μέγιστος αριθμός απο Readers ή Writers). 
Οι Readers επιxειρούν να διαβάσουν την τιμή id του Entrie ενώ οι Writers αυξάνουν την τιμή του id κατα μια μονάδα.
Ένα Entry κρίνεται μη διαθέσιμο όταν ένα process με λειτουργία Writer επιχειρεί να αλλάξει την τιμή του id, ενώ σε κάθε άλλη περίπτωση θεωρρείται διαθέσιμο.
Κάθε process επιχειρεί να εκτελέση την λειτουργία του 4 φορές `#define  TIMES_TO_WORK  4` και κάθε φορά επιλέγεται τυχαία το Entrie που θα κάνει access.

Εκτελώντας το πρόγραμμα με την εντολή `make run` παρατηρόυμε τα εξής αποτελέσματα:
```
./bin/runner 8 10 5 3
Entries at start
1 1 1 1 1 1 1 1 1 1 
Process created: pid = 6748
	Writing on entry: 6, pid:6748
	Writing on entry: 1, pid:6748
	Writing on entry: 2, pid:6748
Process created: pid = 6749
	Writing on entry: 5, pid:6748
	Writing on entry: 1, pid:6749
	Writing on entry: 2, pid:6749
	Writing on entry: 5, pid:6749
Process created: pid = 6750
	Reading entry: 2, value = 3, pid:6750
	Writing on entry: 7, pid:6749
	Reading entry: 5, value = 3, pid:6750
	Reading entry: 7, value = 2, pid:6750
Process created: pid = 6751
	Reading entry: 7, value = 2, pid:6750
	Writing on entry: 5, pid:6751
	Writing on entry: 7, pid:6751
	Writing on entry: 7, pid:6751
	Writing on entry: 8, pid:6751
Process with pid = 6748 exited
Process with pid = 6749 exited
Process with pid = 6750 exited
Process created: pid = 6753
	Reading entry: 7, value = 4, pid:6753
	Reading entry: 8, value = 2, pid:6753
Process with pid = 6751 exited
	Reading entry: 2, value = 3, pid:6753
Process created: pid = 6752
Process created: pid = 6754
	Reading entry: 0, value = 1, pid:6753
	Reading entry: 8, value = 2, pid:6754
Process created: pid = 6755
	Reading entry: 7, value = 4, pid:6752
	Reading entry: 2, value = 3, pid:6754
	Reading entry: 7, value = 4, pid:6752
	Reading entry: 2, value = 3, pid:6755
	Reading entry: 8, value = 2, pid:6752
	Reading entry: 0, value = 1, pid:6754
	Reading entry: 2, value = 3, pid:6752
	Reading entry: 1, value = 3, pid:6754
	Reading entry: 0, value = 1, pid:6755
	Reading entry: 1, value = 3, pid:6755
Process with pid = 6753 exited
	Reading entry: 2, value = 3, pid:6755
Process with pid = 6754 exited
Process with pid = 6752 exited
Process with pid = 6755 exited

Entries at the end
1 3 3 1 1 4 2 4 2 1
```
Διότι δεν είναι ξεκάθαρο πως τα processes δουλεύουν παράλληλα πρόσθεσα μια εντολή `sleep(1);` (src/cordinator.c: 76) έτσι ώστε καθε φοα που ένα process εκτελεί την λειρουργία του, περιμένει για 1 δευτερόλεπτο μέχρι να την εκτελέσει ξανά.
Έτσι έχουμε τα εξής αποτελέσματα:
```
./bin/runner 8 10 5 3
Entries at start
1 1 1 1 1 1 1 1 1 1 
Process created: pid = 7272
	Writing on entry: 8, pid:7272
Process created: pid = 7273
	Writing on entry: 1, pid:7273
Process created: pid = 7274
	Reading entry: 2, value = 1, pid:7274
Process created: pid = 7275
Process created: pid = 7277
Process created: pid = 7276
	Reading entry: 8, value = 2, pid:7277
	Writing on entry: 9, pid:7275
	Reading entry: 6, value = 1, pid:7276
Process created: pid = 7278
	Reading entry: 5, value = 1, pid:7278
Process created: pid = 7279
	Reading entry: 1, value = 2, pid:7279
	Writing on entry: 1, pid:7272
	Writing on entry: 2, pid:7273
	Reading entry: 9, value = 2, pid:7274
	Reading entry: 5, value = 1, pid:7277
	Writing on entry: 6, pid:7275
	Reading entry: 8, value = 2, pid:7276
	Reading entry: 1, value = 3, pid:7278
	Reading entry: 2, value = 2, pid:7279
	Writing on entry: 2, pid:7272
	Writing on entry: 9, pid:7273
	Reading entry: 6, value = 2, pid:7274
	Reading entry: 1, value = 3, pid:7277
	Writing on entry: 8, pid:7275
	Reading entry: 2, value = 3, pid:7278
	Reading entry: 5, value = 1, pid:7276
	Reading entry: 4, value = 1, pid:7279
	Writing on entry: 9, pid:7272
	Writing on entry: 6, pid:7273
	Reading entry: 8, value = 3, pid:7274
	Reading entry: 2, value = 3, pid:7277
	Writing on entry: 5, pid:7275
	Reading entry: 4, value = 1, pid:7278
	Reading entry: 4, value = 1, pid:7279
	Reading entry: 1, value = 3, pid:7276
Process with pid = 7272 exited
Process with pid = 7273 exited
Process with pid = 7274 exited
Process with pid = 7275 exited
Process with pid = 7276 exited
Process with pid = 7277 exited
Process with pid = 7278 exited
Process with pid = 7279 exited

Entries at the end
1 3 3 1 1 2 3 1 3 4
```
Τελός, αποσχολιάζοντας τις γραμμές src:fnclin.c:30 και src:fnclin.c:41 και εκτελώντας το πρόγραμμα μπορούμε να δούμε εμφανέστατα την λειτουργία του semaphore!!

