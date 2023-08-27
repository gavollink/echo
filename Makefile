CC=gcc
CCFLAGS=-Wall -O2
CCFLAGS+=-std=gnu99
#CCFLAGS+=-ggdb

echo: echo.c
	$(CC) $(CCFLAGS) -o echo echo.c

clean:
	=rm -f echo echo.o
