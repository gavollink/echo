/****************************************************************************
 * echo.c - cloning what BASH does in `NOT POSIX' mode
 *
 * License embedded at bottom of this file.
 *
 */
#define VERSION "0.01"
#define C_YEARS "2023"
#define IDENT "echo"
#define SUBHEAD " (another clone)"
#define AT "@"
#define DOT "."
#define AUTHOR1 "Gary Allen Vollink"
#define CONTACT1 "gary" AT "vollink" DOT "com"
#define AUTHORS "   " AUTHOR1 " <" CONTACT1 ">\n"

/* WHY FOUR SPACES?
 * Four is the minimum default string-length that `strings' will show.
 */
const char LICENSE[] = "\
    \n\
LICENSE BLOCK\n\
    \n\
MIT License\n\
    \n\
" IDENT " -- https://gitlab.home.vollink.com/external/echo/\n\
Copyright (c) " C_YEARS ", by:\n\
" AUTHORS
"    \n\
Permission is hereby granted, free of charge, to any person obtaining\n\
a copy of this software and associated documentation files\n\
(the \"Software\"), to deal in the Software without restriction,\n\
including without limitation the rights to use, copy, modify, merge,\n\
publish, distribute, sublicense, and/or sell copies of the Software,\n\
and to permit persons to whom the Software is furnished to do so,\n\
subject to the following conditions:\n\
    \n\
The above copyright notice and this permission notice shall be\n\
included in all copies or substantial portions of the Software.\n\
    \n\
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n\
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n\
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\n\
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY\n\
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,\n\
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE\n\
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
    \n\
END LICENSE BLOCK\n\
    \n\
";

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* WHY FOUR SPACES?
 * Four is the minimum default string-length that `strings' will show.
 */
const char HELP_TEXT[] = "\
    \n\
HELP TEXT BLOCK\n\
    \n\
" IDENT SUBHEAD " Version: " VERSION"\n\
  https://gitlab.home.vollink.com/external/echo/\n\
    \n\
Echo the arguments to standard output.\n\
Tested to be functionaly identical to the Bash shell built-in,\n\
when Bash is NOT stuck into `POSIX' mode.\n\
    \n\
    -n  Suppress sending a newline at the end.\n\
    -e  Interpret `\\?' escape sequences.\n\
    -E  Do NOT interpret `\\?' escape sequences\n\
        -eE would leave it off.\n\
        -Ee would turn it on.\n\
    \n\
If -e is in effect, the following (and ONLY these) escape sequences\n\
will be recognized:\n\
    \n\
        \\\\        backslash    (ASCII 92 | \\x5c | \\0134)\n\
        \\a        Bell         (ASCII 7)\n\
        \\b        Backspace    (ASCII 8)\n\
        \\c        Stop output and exit immediate (no newline)\n\
        \\e        Escape       (ASCII 27 | \\x1b | \\033)\n\
        \\f        Form Feed    (ASCII 12 | \\x0c | \\014)\n\
        \\n        Newline      (ASCII 10 | \\x0a | \\012)\n\
        \\n        Return       (ASCII 13 | \\x0d | \\015)\n\
        \\t        Tab          (ASCII  9 | \\x09 | \\011)\n\
        \\v        Vertical Tab (ASCII 11 | \\x0b | \\013)\n\
        \\0NNN     Byte designated in Octal (1 to 3 digits)\n\
        \\xNN      Byte designated in Hexidecimal (1 or 2 digits)\n\
    \n\
NOTE1: This program has no ENVIRONMENT variable side effects.\n\
NOTE2: Using `strings' is the best way to see this text or the\n\
     LICENSE BLOCK, the program has no way to output either.\n\
    \n\
END HELP TEXT BLOCK\n\
    \n\
";

const char WHAT[] = "    \n\
THIS PROGRAM IS " IDENT ", Version: " VERSION
" by: \n" AUTHORS "\
    \n\
";

#define ERROR 0x80000000
#define NO_NEWLINE 0x2
#define INTERPRET_ESC 0x4

int
sncmp( const char * a, const char * b, long int len )
{
    int match = 0;
    for ( long int cx = 0; cx < len; cx++ ) {
        if ( ( 0 == a[cx] ) && ( 0 == b[cx] ) ) {
            return( match );
        }
        else if ( 0 == a[cx] ) {
            match--;
            return( match );
        }
        else if ( 0 == b[cx] ) {
            match++;
            return( match );
        }
        match += ( a[cx] - b[cx] );
        if ( match ) {
            return( match );
        }
    }
    return match;
}

int
opt ( char * ostring )
{
    int retval = 0;
    /* skip the first - */
    for ( int cx = 1; cx < strlen( ostring ); cx++ ) {
        switch ( ostring[cx] )
        {
            case 'n':
                retval = retval | NO_NEWLINE;
                break;
            case 'e':
                retval = retval | INTERPRET_ESC;
                break;
            case 'E':
                /* Had to look up bitwise NOT ~ */
                retval = retval & ( ~INTERPRET_ESC );
                break;
            default:
                retval = retval | ERROR;
                break;
        }
    }
    return retval;
}

int
hextobin ( const unsigned char h )
{
    switch ( h )
    { /* No break, everything returns */
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'a':
        case 'A':
            return 10;
        case 'b':
        case 'B':
            return 11;
        case 'c':
        case 'C':
            return 12;
        case 'd':
        case 'D':
            return 13;
        case 'e':
        case 'E':
            return 14;
        case 'f':
        case 'F':
            return 15;
    }
    return 0;
}

int
doprint( int opts, char* out )
{
    int buildval = 0;
    int retval = 0;
    for ( int cx = 0; cx < strlen( out ); cx++ ) {
        if ( opts & INTERPRET_ESC ) {
            if ( '\\' == out[cx] ) {
                if ( 0 == sncmp ( "\\\\", &out[cx], 2 ) ) {
                    putchar('\\');
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\a", &out[cx], 2 ) ) { // BEL
                    putchar(7);
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\b", &out[cx], 2 ) ) { // BS
                    putchar(8);
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\c", &out[cx], 2 ) ) {
                    /* TO match `bash` built-in, I'm leaving this out...
                    if ( 0 == ( opts & NO_NEWLINE ) ) {
                        putchar('\n');
                    } **** */
                    exit(0);
                }
                else if ( 0 == sncmp ( "\\e", &out[cx], 2 ) ) { // ESC
                    /* Escape Character */
                    putchar( 0x1b );
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\f", &out[cx], 2 ) ) { // FF
                    putchar( 0x0c );
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\n", &out[cx], 2 ) ) { // NL
                    putchar( 0x0a );
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\r", &out[cx], 2 ) ) { // CR
                    putchar( 0x0d );
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\t", &out[cx], 2 ) ) { // HT
                    putchar( '\t' );
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\v", &out[cx], 2 ) ) { // VT
                    putchar( 0x0b );
                    retval++;
                    cx++;
                }
                else if ( 0 == sncmp ( "\\x", &out[cx], 2 ) ) { // HEX
                    if ( isxdigit( out[cx+2] ) ) {
                        if ( isxdigit( out[cx+3] ) ) {
                            buildval = 16 * hextobin( out[cx+2] );
                            buildval = buildval + hextobin( out[cx+3] );
                            cx = cx + 3;
                        } else {
                            buildval = hextobin( out[cx+2] );
                            cx = cx + 2;
                        }
                        if ( buildval ) {
                            putchar( buildval );
                            retval++;
                        }
                        buildval = 0;
                    } else {
                        putchar( '\\' );
                        retval++;
                    }
                }
                else if ( 0 == sncmp ( "\\0", &out[cx], 2 ) ) { // OCTAL
                    if ( '0' <= out[cx+2] && '7' >= out[cx+2] ) {
                        cx++;
                        for ( int dx = 0; dx < 3; dx++ ) {
                            if ( '0' <= out[cx+1] && '7' >= out[cx]+1 ) {
                                buildval =
                                    (buildval * 8) + hextobin( out[cx+1] );
                                cx++;
                            } else {
                                dx=3;
                            }
                        }
                        if ( buildval ) {
                            putchar( buildval );
                            retval++;
                        }
                        buildval = 0;
                    } else {
                        putchar( '\\' );
                        retval++;
                    }
                } else {
                    putchar( '\\' );
                    retval++;
                }
            } else {
                putchar( out[cx] );
                retval++;
            }
        } else {
            putchar( out[cx] );
            retval++;
        }
    }
    return retval;
}

void
defeat_optimization(void)
{
    /* ****
     * May be silly, but aggressive compiler optimization will
     * remove the text blocks at the top of this file, so
     * this function uses them and assign to eachother in a loop
     * to trick optimizers into leaving them alone..
     * ***
     * It is possible that a sufficiently advanced optimizer might
     * notice that this function has no modified output and kill
     * the text anyway, but I've yet to find that.
     */
    char *keep_WHAT_from_optimizing_out = (char *)WHAT;
    char *keep_HELP_TEXT_from_optimizing_out = (char *)HELP_TEXT;
    char *keep_LICENSE_from_optimizing_out = (char *)LICENSE;
    char *keep_from_optimizing_out = 1 + keep_WHAT_from_optimizing_out;
    keep_WHAT_from_optimizing_out = 1 + keep_HELP_TEXT_from_optimizing_out;
    keep_HELP_TEXT_from_optimizing_out = 1 + keep_LICENSE_from_optimizing_out;
    keep_LICENSE_from_optimizing_out = keep_from_optimizing_out;
}

int
main ( int argc, char *argv[] )
{
    int opt_count = 1;
    int opts = 0;
    int print = 0;

    defeat_optimization();

    for ( int cx = opt_count; cx < argc; cx++ ) {
        if ( 0 == print ) {
            if ( '-' == argv[cx][0] ) {
                int thisopt = opt( argv[cx] );
                if ( thisopt & ERROR ) {
                    print = 1;
                } else {
                    // To act the same as bash's builtin, kill 
                    // all letters from a failed optoin
                    opts = opts | thisopt;
                    opt_count++;
                }
            } else {
                print = 1;
            }
        }
        if ( 1 == print ) {
            if ( opt_count < cx ) {
                /* Print a space between args */
                putchar(' ');
            }
            doprint( opts, argv[cx] );
        }
    }

    if ( 0 == ( opts & NO_NEWLINE ) ) {
        putchar('\n');
    }
    
    exit (0);
}

/* EOF echo.c */
