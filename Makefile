CC=gcc
CCFLAGS=-Wall -O2
CCFLAGS+=-std=gnu99
#CCFLAGS+=-ggdb

ARCH=$(shell uname -ms)

ifeq ($(ARCH), Darwin arm64)
echo: universal.echo
	cp universal.echo echo

else ifeq ($(ARCH), Darwin x86_64)
echo: x86_64.echo
	cp x86_64.echo echo

else
echo: vanilla.echo
	cp vanilla.echo echo

endif

universal.echo: x86_64.echo arm64.echo
	lipo -create -output universal.echo x86_64.echo arm64.echo

x86_64.echo: echo.c
	$(CC) $(CCFLAGS) -o x86_64.echo echo.c -target x86_64-apple-macos10.10

arm64.echo: echo.c
	$(CC) $(CCFLAGS) -o arm64.echo echo.c -target arm64-apple-macos11

vanilla.echo: echo.c
	$(CC) $(CCFLAGS) -o vanilla.echo echo.c

clean:
	-rm -f echo echo.o vanilla.echo x86_64.echo arm64.echo universal.echo
