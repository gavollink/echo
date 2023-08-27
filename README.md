# echo

FUNCTIONAL CLONE of Bash's built-in echo, but ALWAYS HAS `-e` support
(where Bash and even GNU echo can be put into POSIX mode with no `-e` support).

# Compile

Requires: gcc (or macOS `Xcode Command Line Tools`)
the full Xcode isn't actually necessary.

```$ gcc -Wall -O2 -std=gnu99 -o echo echo.c```

# Install

Put it where it makes sense for you.

# Try This

```$ strings echo | less```

This goes through the `echo` command and prints out strings it finds
inside that file.  An embedded HELP and LICENSE show up here.

# Why

One day, I'm on macOS and I decide to try the `KiTTY` terminal.
I start it up, and my `PROMPT_COMMAND` script starts dumping junk
characters.  Turns out that the first thing KiTTY does on startup
is to force bash into POSIX mode.  When that happens, the built-in
`echo` command stops interpreting -e, uh, I needed that.  I use
it in several places during my `.profile` and `.bashrc` startup
scripts.

Okay, what about macOS /bin/echo?  No, that is strict `POSIX.1`.
(doesn't interpret -e, only -n)

Thus this clone of `echo` has been carefully written to do the same
thing as the Bash shell's built-in `echo` (when Bash is NOT in POSIX
mode).

Why not just use the Gnu version?

Gnu echo is part of coreutils and requires a whole bunch of garbage,
including the gnulib package (which in turn requires a handful of
other things).

Also, if something sets the environment variable `POSIXLY_CORRECT=1`
then even GNU echo stops responding to `-e`.

This version is under 400 lines, and doesn't need any extra libraries.

