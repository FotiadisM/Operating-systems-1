CC = gcc
CFLAGS = -g3 -Wall -pthread

BDIR = bin
ODIR = build
IDIR = include
SDIR = src

EXECUTABLE = runner

_DEPS = entrie.h fnclib.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
# DEPS = $(IDIR)/$(wildcard *.h)

_OBJ = cordinator.o fnclib.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
# OBJ = $(ODIR)/$(wildcard *.o)

# SRC = $(SDIR)/$(wildcard *.c)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean run

run:
	./$(BDIR)/$(EXECUTABLE) 8 10 5 3

clean:
	rm -f $(ODIR)/*.o
	rm -f $(BDIR)/$(EXECUTABLE)