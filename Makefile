CC = gcc
CFLAGS = -g3
SDIR = src
BDIR = bin

IDIR = include
_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

ODIR = build
_OBJ = 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@  $< $(CFLAGS)

$(BDIR)/runner: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f $(BDIR)/runner