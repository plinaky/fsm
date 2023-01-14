IDIR =../fsm

CC=gcc

CFLAGS=-I$(IDIR)

DEPS = ../fsm/fsm.h ./scan.h

OBJ = no_comment.o include.o preproc.o scan.o

TARGET = $(patsubst %.o,%,$(OBJ))

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)

%: %o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean all

clean:
	rm -Rf $(TARGET) $(OBJ)
