CC = gcc
CFLAGS = -g3 -Wall

BDIR = bin
ODIR = build
IDIR = include
SDIR = src

EXECUTABLE = runner

_DEPS =	
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
# DEPS = $(IDIR)/$(wildcard *.h)

_OBJ = cordinator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
# OBJ = $(ODIR)/$(wildcard *.o)

# SRC = $(SDIR)/$(wildcard *.c)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean run

run:
	./$(BDIR)/$(EXECUTABLE)

clean:
	rm -f $(ODIR)/*.o
	rm -f $(BDIR)/$(EXECUTABLE)