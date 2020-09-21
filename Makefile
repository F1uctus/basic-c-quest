CC      = gcc
CFLAGS  = -g -std=c11
RM      = rm -f


default: all

all: main

rpg: main.c
	$(CC) $(CFLAGS) ./*.c -l ncurses -l form -o main

run: rpg
	./main

clean:
	$(RM) main
