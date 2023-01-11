IDIR =.

CC=gcc

CFLAGS=-I$(IDIR)

DEPS = graph.h list.h

OBJ = graph.o

all: $(OBJ)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean all

clean:
	rm -Rf $(OBJ)
