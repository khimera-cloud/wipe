#Makefile for wipe

CC:=gcc -O2 -Wall -fanalyzer -Wl,--build-id=none
BINS:=wipe
OBJS:=wipe.o
STRIP:=strip -s

.PHONY: clean distclean strip all

default: $(BINS)

all: strip

%.o: %.c %.h
	$(CC) -c $< -o $@

wipe: $(OBJS)
	$(CC) $^ $(LIBS) -o $@

strip: $(BINS)
	$(STRIP) $^

clean:
	rm -f $(BINS) $(OBJS)

distclean: clean
	rm -f *.out
