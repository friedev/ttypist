PROGRAM = ttypist
INSTALL_TARGETS = $(PROGRAM) ttypist-gen

CFLAGS  = -g -Wall -Wextra -Wpedantic
LDFLAGS = -lncurses

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

PREFIX     ?= /usr/local
BIN_PREFIX ?= $(DESTDIR)$(PREFIX)/bin

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS) $(PROGRAM)

.PHONY: install
install: all
	install $(INSTALL_TARGETS) $(BIN_PREFIX)

.PHONY: uninstall
uninstall:
	$(RM) $(addprefix $(BIN_PREFIX)/,$(INSTALL_TARGETS))
