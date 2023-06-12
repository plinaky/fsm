
CC     = gcc

IDIR   = ../fsm ../data

CFLAGS = $(patsubst %,-I%,$(IDIR))

CFLAGS+= -ggdb

DEPS   = ../fsm/fsm.h ../data/list.h

OBJ    = comment.o emptyline.o

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
