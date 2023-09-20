# echo

FUNCTIONAL CLONE of Bash's built-in echo, except it ALWAYS
HAS `-e` support (where Bash and even GNU echo can be
put into POSIX mode with no `-e` support).

Written in Rust, as a first project.

## Compile

Requires: rustc and cargo ( https://www.rust-lang.org/tools/install )

```
$ cd c
$ cargo build --release
```

## Install

Put rust/target/release/echo where it makes sense for you.

## Compatibility

On `macOS` Yosemite (at least) through Ventura (so far),
the `bash` built-in `echo` does NOT expand the escape, `\e`,
even though the man page claims it does.

For simplicity, this follows the macOS bash manpage, not the actual
behavior.  I really hate that I had to think about this and write it
down.  It is hard to be someone who likes developing on macOS, when
they let these types of basics go unfixed for YEARS.

IF this is important to YOU:

Modify the file rust/src/main.rs.  Remove the five lines marked
`// MAN PAGE BEHAVIOR`
Unlike the C version, I haven't found an equivalent to compile-time define
flags for cargo/rust.

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

This Rust version is under 400 lines, and doesn't need extra libraries.

## Why Rust?

I want to figure out a bit about how Rust works by doing something I've
already done in C.
If you don't like Rust, I wrote a C version, first.
It's one directory over, use that instead.

