
CC     = gcc

IDIR   = ../fsm ../data_struct

CFLAGS = $(patsubst %,-I%,$(IDIR))

DEPS   = ../fsm/fsm.h ../data_struct/list.h

OBJ    = comment.o include.o preproc.o scan.o

TARGET = $(patsubst %.o,%,$(OBJ))

INC    = $(patsubst %.o,%.h,$(OBJ))

DEPS  += INC



%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all: $(TARGET)


%: %o
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean all


clean:
	rm -Rf $(TARGET) $(OBJ)
