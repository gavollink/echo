# echo

FUNCTIONAL CLONE of Bash's built-in echo, except it ALWAYS
HAS `-e` support (where Bash and even GNU echo can be
put into POSIX mode with no `-e` support).

## Compile

Requires: gcc (or macOS `Xcode Command Line Tools`)
the full Xcode isn't actually necessary for this.

```
$ gcc -Wall -O2 -std=gnu99 -o echo echo.c
```

## Install

Put it where it makes sense for you.

## Compatibility

On `macOS` Yosemite through Ventura, the `bash` built-in `echo`
does NOT expand the escape, `\e`, but the man page claims it does.

For simplicity, this follows the macOS bash manpage, not the actual
behavior.  I really hate that I had to think about this and write it
down.  It is hard to be someone who likes developing on macOS, when 
they let these types of basics go unfixed for YEARS.

IF this is important to YOU:

```
gcc -DMAC_BROKEN_BASH -Wall -O2 -std=gnu99 -o echo echo.c
```

I literally put an #ifndef around the `if ( "\\e" ...` block, so it
will do exactly as the macOS broken Bash that doesn't follow its own
man page.

## Try This

```
$ strings echo | less
```

This goes through the `echo` command and prints out strings it finds
inside that file.  An embedded HELP and LICENSE show up here along with
the version number.

For compatibility sake I couldn't add `--help` and `--version` like the
Gnu echo tool does (again, only when `POSIXLY_CORRECT` doesn't force
it to ignore those AND `-e` as well.  This is what I could do.

NOTE: If this is built on MacOS 11 or newer, it will default to building
a Universal (`x86_64` and `arm64`) binary, but that means that those
HELP and LICENSE blocks will be repeated twice.  This is expected
because of how Universal binaries are built.

# Why

One day, I'm on macOS and I decide to try the `KiTTY` terminal.
I start it up, and my `PROMPT_COMMAND` script starts dumping junk
characters.  Turns out that the first thing KiTTY does on startup
is to force bash into POSIX mode.  When that happens, the built-in
`echo` command stops interpreting `-e`, uh, I needed that.  I use
it in several places during my `.profile` and `.bashrc` startup
scripts.

Okay, what about macOS /bin/echo?  No, that is strict `POSIX.1`.
It doesn't interpret `-e`, only `-n`.

Thus this clone of `echo` has been carefully written to do the same
thing as the Bash shell's built-in `echo` (when Bash is NOT in POSIX
mode).

Why not just use the Gnu version?

Gnu echo is part of coreutils and requires a whole bunch of garbage,
including the gnulib package (which in turn requires a handful of
other things).

Also, if something sets the environment variable `POSIXLY_CORRECT=1`
then even GNU echo stops responding to `-e`.  By the way, when bash is
started in POSIX mode, it makes sure that environment variable IS set.

This version is under 500 lines, and doesn't need extra libraries.

