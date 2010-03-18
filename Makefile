CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lncurses

INCLUDES = -I/usr/lib/oss/include/sys

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

all: osscmix

osscmix: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

clean:
	rm -rf *.o
distclean: clean
	rm -rf osscmix


