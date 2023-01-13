IDIR =../fsm

CC=gcc

CFLAGS=-I$(IDIR)

DEPS = ../fsm/fsm.h

#OBJ = no_comment.o
OBJ = include.o

TARGET = $(patsubst %.o,%,$(OBJ))

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean all

clean:
	rm -Rf $(TARGET) $(OBJ)
