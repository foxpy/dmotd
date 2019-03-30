CC = clang
CFLAGS = -Wall -Wextra -pedantic -std=gnu99 -D_FORTIFY_SOURCE=2 -O2
TARGET = dmotd

PREFIX := /usr/local

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $<
	strip $@

install:
	install -Dm755 $(TARGET) '$(PREFIX)/bin'

uninstall:
	rm '$(PREFIX)/bin/$(TARGET)'

clean:
	rm -fv $(TARGET) $(OBJECTS)
