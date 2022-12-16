IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj

LIBS=

_DEPS = fsm.h graph.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_GOBJ = graph.o test.o
GOBJ = $(patsubst %,$(ODIR)/%,$(_GOBJ))

_FOBJ = fsm.o
FOBJ = $(patsubst %,$(ODIR)/%,$(_FOBJ))

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

all: test fsm

test: $(GOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

fsm: $(FOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean all


clean:
	rm -Rf $(ODIR) *~ graph fsm
