
CC     = gcc

IDIR   = .

CFLAGS = $(patsubst %,-I%,$(IDIR))

DEPS   = ./heapstack.h

OBJ    = heapstack.o

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
