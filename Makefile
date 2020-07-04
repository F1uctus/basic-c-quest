CC      = gcc
CFLAGS  = -g -std=c11
RM      = rm -f


default: all

all: rpg

rpg: main.c
	$(CC) $(CFLAGS) ./*.c -l ncurses -l form -o rpg

run: rpg
	./rpg

clean:
	$(RM) rpg
