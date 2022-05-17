CFLAGS=-Wall -Wextra -Wpedantic
LDFLAGS=-lncurses

all: ttypist

ttypist: ttypist.c
	$(CC) $(CFLAGS) $? $(LDFLAGS) -o $@

clean:
	$(RM) ttypist
