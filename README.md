# echo

FUNCTIONAL CLONE of Bash's built-in echo, except it ALWAYS
HAS `-e` support (where Bash and even GNU echo can be
put into POSIX mode with no `-e` support).

# Rust and C

These are two completely different project folders.
They are independent, and have been tested to be sure they
behave the same way.

The approach was slightly different with each, partly because of
the way Rust is very protective about variable ownership.

# Requires...

The C version requires gcc gnu99 compatible compiler.  That is
any gcc on Linux or Apple OS X.  On Apple OS X, you only need the
`Xcode Command Line Tools`, not the whole Xcode package.

The Rust version requires cargo and rustc that supports spec 2021,
though you may have luck adjusting that down, as this is my first
Rust project, I've left it as is.

# Differences...

The C echo is version 0.03.
The Rust echo is version 0.0.2.

I've found no functional differences at all, and I do some silly
tests.

# More

See near identical README files in `rust` and `c` directories.
