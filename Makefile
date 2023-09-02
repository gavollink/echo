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

FINAL=echo
SOURCE=echo.c
X_DEPS=Makefile

SYS=$(shell uname -s)
ARCH=$(shell uname -m)
MAJ_VER=$(shell uname -r | awk -F. '{print $$1}')

ifeq ($(shell test "Darwin" = $(SYS) -a $(MAJ_VER) -ge 20; echo $$?), 0)
# Darin 20 is Big Sur:
# Big Sur is the earliest macOS that supported arm64.
# Anything (x86_64 or arm64) will be able to target
# a universal binary with arm64-Big Sur and x86_64-Yosemite
INTERIM=universal.$(FINAL)
else ifeq ($(shell test "Darwin" = $(SYS) -a $(MAJ_VER) -lt 14; echo $$?), 0)
# Darwin 14 is Yosemite:
# Yosemite is the earlies target macOS I've been able to test,
# so if Darwin is from Maveriks or older, we'll treat this like
# Linux, and just take compiler target defaults (whatever they are)
INTERIM=default.$(FINAL)
else ifeq ($(shell test "Darwin" = $(SYS); echo $$?), 0)
# x86_64 from Yosemite to Moneray will target Yosemite as the
# widest target
# arm64 always targets Big Sur.
INTERIM=$(ARCH).$(FINAL)
else
INTERIM=default.$(FINAL)
endif

$(FINAL): $(INTERIM)
	cp $(INTERIM) $(FINAL)

universal.$(FINAL): x86_64.$(FINAL) arm64.$(FINAL)
	lipo -create -output universal.$(FINAL) x86_64.$(FINAL) arm64.$(FINAL)

x86_64.$(FINAL): $(SOURCE) $(X_DEPS)
	$(CC) $(CCFLAGS) -o x86_64.$(FINAL) $(SOURCE) -target x86_64-apple-macos10.10

arm64.$(FINAL): $(SOURCE) $(X_DEPS)
	$(CC) $(CCFLAGS) -o arm64.$(FINAL) $(SOURCE) -target arm64-apple-macos11

default.$(FINAL): $(SOURCE) $(X_DEPS)
	$(CC) $(CCFLAGS) -o default.$(FINAL) $(SOURCE)

clean:
	-rm -f echo echo.o *.echo
