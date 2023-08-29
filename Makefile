CC=gcc
CCFLAGS=-Wall -O2
CCFLAGS+=-std=gnu99
ifdef MAC_BROKEN_BASH
CCFLAGS+=-DMAC_BROKEN_BASH
endif
ifdef DEBUG
	# Maintainer stuff only, you don't want to see this.
CCFLAGS+=-DDEBUG
endif
#CCFLAGS+=-ggdb

SYS=$(shell uname -s)
ARCH=$(shell uname -m)
MAJ_VER=$(shell uname -r | awk -F. '{print $$1}')

ifeq ($(shell test "Darwin" = $(SYS) -a $(MAJ_VER) -ge 20; echo $$?), 0)
ECHO_DEP=universal.echo
else ifeq ($(shell test "Darwin" = $(SYS); echo $$?), 0)
ECHO_DEP=$(ARCH).echo
else
ECHO_DEP=default.echo
endif

echo: $(ECHO_DEP)
	cp $(ECHO_DEP) echo

universal.echo: x86_64.echo arm64.echo
	lipo -create -output universal.echo x86_64.echo arm64.echo

x86_64.echo: echo.c
	$(CC) $(CCFLAGS) -o x86_64.echo echo.c -target x86_64-apple-macos10.10

arm64.echo: echo.c
	$(CC) $(CCFLAGS) -o arm64.echo echo.c -target arm64-apple-macos11

default.echo: echo.c
	$(CC) $(CCFLAGS) -o default.echo echo.c

clean:
	-rm -f echo echo.o *.echo
