
CC     = gcc

IDIR   = .

CFLAGS = $(patsubst %,-I%,$(IDIR))

OBJ    = test.o

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
