# echo

FUNCTIONAL CLONE of Bash's built-in echo, except it ALWAYS
HAS `-e` support (where Bash and even GNU echo can be
put into POSIX mode with no `-e` support).

Written in C, which I've been writing since 1994.

## Compile

Requires: gcc (or macOS `Xcode Command Line Tools`)
the full Xcode isn't actually necessary for this.

```
$ cd c
$ gcc -Wall -O2 -std=gnu99 -o echo echo.c
```

## Install

Put c/echo where it makes sense for you.

If you want, you can modify configure.mk to
get `make install` functionality, the stub defines are there
(after make is run at least once).

## Compatibility

On `macOS` Yosemite (at least) through Ventura (so far),
the `bash` built-in `echo` does NOT expand the escape, `\e`,
even though the man page claims it does.

For simplicity, this follows the macOS bash manpage, not the actual
behavior.  I really hate that I had to think about this and write it
down.  It is hard to be someone who likes developing on macOS, when
they let these types of basics go unfixed for YEARS.

IF this (broken) behavior is important to YOU:

```
gcc -DMAC_BROKEN_BASH -Wall -O2 -std=gnu99 -o echo echo.c
```

Or set `MAC_BROKEN_BASH=1` in configure.mk
(which exists after make is run at least one time).

I put an #ifndef around the `if ( "\\e" ...` block, so it
will do exactly as the macOS broken Bash that doesn't follow its own
man page, once the define is set.

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

NOTE: If this is built on MacOS 11 or newer, the build process will
build a universal library.  However, these embedded strings will be
supressed in the Alternate intermediate build.  That is, if built on an M1,
these strings will be surpressed in the `x86_64.echo` intermediate target,
and vice-versa.

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

## Why not just use the Gnu version?

When `bash` is started in POSIX mode, it puts `POSIXLY_CORRECT=1`
in the environment.  Like MANY tools, what that variable is set,
GNU echo forgets most of its features, including `-e`.

GNU echo also is part of coreutils and requires a whole bunch of
other libraries, including the gnulib package
(which in turn requires a handful of other things).

This C version is under 500 lines, and doesn't need extra libraries.

## Why not just use printf?

You got me there, sort of.
Would you believe if I said it was faster to write this, than to change
`echo` to `printf` AND add `\n` to every place echo would have done it?

## Why C?

I've been writing in C since 1994.  So, believe it or not, this was the
fastest way for me to get what I wanted done.
If you don't like it, I got around to the Rust version, too.
It's one directory over, use that instead.

