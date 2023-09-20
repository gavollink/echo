use std::process::exit;
use std::process;
use std::env;
use std::print;

const VERSION: &str = "\necho version 1.02\n";
const LICENSE: &str = "
    \n
LICENSE BLOCK\n
    \n
MIT License\n
    \n
echo -- https://gitlab.home.vollink.com/external/echo/\n
Copyright (c) 2023, by:\n
  Gary Allen Vollink <firstname @ lastname . com>\n
    \n
Permission is hereby granted, free of charge, to any person obtaining\n
a copy of this software and associated documentation files\n
(the \"Software\"), to deal in the Software without restriction,\n
including without limitation the rights to use, copy, modify, merge,\n
publish, distribute, sublicense, and/or sell copies of the Software,\n
and to permit persons to whom the Software is furnished to do so,\n
subject to the following conditions:\n
    \n
The above copyright notice and this permission notice shall be\n
included in all copies or substantial portions of the Software.\n
    \n
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\n
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY\n
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,\n
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE\n
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n
    \n
END LICENSE BLOCK\n
    \n
";
const HELP_TEXT: &str = "
    \n
HELP TEXT BLOCK\n
    \n
echo (another clone) Version: 1.02\n
  https://gitlab.home.vollink.com/external/echo/\n
    \n
Echo the arguments to standard output.\n
Tested to be functionaly identical to the (Linux) Bash shell \n
built-in echo, when Bash is NOT stuck into `POSIX' mode.\n
    \n
    -n  Suppress sending a newline at the end.\n
    -e  Interpret `\\?' escape sequences.\n
    -E  Do NOT interpret `\\?' escape sequences\n
        -eE would leave it off.\n
        -Ee would turn it on.\n
    \n
If -e is in effect, the following (and ONLY these) escape sequences\n
will be recognized:\n
    \n
        \\\\        backslash    (ASCII 92 | \\x5c | \\0134)\n
        \\a        Bell         (ASCII 7)\n
        \\b        Backspace    (ASCII  8 | \\x08 | \\0010)\n
        \\c        Stop output and exit immediate (no newline)\n
        \\e        Escape       (ASCII 27 | \\x1b | \\0033)\n
        \\f        Form Feed    (ASCII 12 | \\x0c | \\0014)\n
        \\n        Newline      (ASCII 10 | \\x0a | \\0012)\n
        \\r        Return       (ASCII 13 | \\x0d | \\0015)\n
        \\t        Tab          (ASCII  9 | \\x09 | \\0011)\n
        \\v        Vertical Tab (ASCII 11 | \\x0b | \\0013)\n
        \\0NNN     Byte designated in Octal (1 to 3 digits)\n
        \\xNN      Byte designated in Hexidecimal (1 or 2 digits)\n
    \n
NOTE1: This program has no ENVIRONMENT variable side effects.\n
NOTE2: Using `strings' is the best way to see this text or the\n
     LICENSE BLOCK, the program has no way to output either.\n
    \n
END HELP TEXT BLOCK\n
    \n
";


struct Config {
    primary: String,
    rest: Vec<String>,
    newline: bool,
    escapes: bool,
}

impl Config {
    pub fn build() -> Result<Config, &'static str> {
        let args: Vec<String> = env::args().collect();
        let mut rest: Vec<String> = [].to_vec();
        let arg_vec = args.iter();
        let mut primary: String = "".to_string();
        let mut argcheck: bool = true;
        let mut newline: bool = true;
        let mut found_self: bool = false;
        let mut got_primary: bool = false;
        let mut escapes: bool = false;

        'arg: for arg in arg_vec {
            if ! found_self {
                found_self = true;
                continue;
            }
            if true == argcheck {
                let char_vec: Vec<char> = arg.chars().collect();
                if '-' == char_vec[0] {
                    'cx: for cx in 1..char_vec.len() {
                        if 'n' == char_vec[cx] {
                            newline = false;
                        }
                        else if 'e' == char_vec[cx] {
                            escapes = true;
                        }
                        else if 'E' == char_vec[cx] {
                            escapes = false;
                        }
                        else {
                            argcheck = false;
                            break 'cx;
                        }
                    }
                }
                else {
                    argcheck = false;
                }
                if true == argcheck {
                    continue 'arg;
                }
            }
            if ! got_primary {
                primary = arg.clone();
                got_primary = true;
            }
            else {
                rest.push( (*arg.clone()).to_string() );
            }
        }

        Ok(Config {
            primary,
            rest,
            newline,
            escapes,
        })
    }
}

fn do_escape( input: &String ) -> ( bool, String ) {
    let svec: Vec<char> = input.chars().collect();
    let mut skip = 0;
    let mut output: String = "".to_owned();
    let mut bail: bool = false;

    'cx: for cx in 0..svec.len() {
        if 0 < skip {
            skip -= 1;
            continue 'cx;
        }
        if '\\' == svec[cx] {
            let next = svec[cx+1];
            match next {
                '\\' => {   // Literal backslash
                    output.push_str( "\\" );
                    skip += 1;
                    continue 'cx;
                },
                'a' => {    // BEL
                    output.push_str( "\x07" );
                    skip += 1;
                    continue 'cx;
                },
                'b' => {    // BS
                    output.push_str( "\x08" );
                    skip += 1;
                    continue 'cx;
                },
                'c' => {    // BAILOUT
                    bail = true;
                    break 'cx;
                },
                'e' => {    // ESC              // MAN PAGE BEHAVIOR
                    output.push_str( "\x1b" );  // MAN PAGE BEHAVIOR
                    skip += 1;                  // MAN PAGE BEHAVIOR
                    continue 'cx;               // MAN PAGE BEHAVIOR
                },                              // MAN PAGE BEHAVIOR
                'f' => {    // FF
                    output.push_str( "\x0c" );
                    skip += 1;
                    continue 'cx;
                },
                'n' => {    // NL
                    output.push_str( "\x0a" );
                    skip += 1;
                    continue 'cx;
                },
                'r' => {    // NL
                    output.push_str( "\x0d" );
                    skip += 1;
                    continue 'cx;
                },
                't' => {    // HT
                    output.push_str( "\t" );
                    skip += 1;
                    continue 'cx;
                },
                'v' => {    // VT
                    output.push_str( "\x0b" );
                    skip += 1;
                    continue 'cx;
                },
                'x' => {    // HEX
                    let mut dx: u32 = 2;
                    let mut dig: bool = true;
                    let mut mult: u32 = 1;
                    let mut val: u32 = 0;
                    while dig {
                        if svec[cx+dx as usize].is_digit(16) {
                            val *= mult;
                            val = val + svec[cx+dx as usize]
                                .to_digit(16)
                                    .expect("Internal error on hex convert.");
                            if 2 == dx {
                                skip += 1;
                                mult = 16;
                            }
                            else {
                                dig = false;
                            }
                            skip += 1;
                            dx += 1;
                        } else {
                            dig = false;
                        }
                    }
                    if 0 != skip {
                        output.push_str(
                            &char::from_u32(val)
                                .expect("Internal error on hex convert")
                                    .to_string()
                                       );
                    }
                    continue 'cx;
                },
                '0' => {    // OCT
                    let mut dx: u32 = 2;
                    let mut dig: bool = true;
                    let mut mult: u32 = 1;
                    let mut val: u32 = 0;
                    while dig {
                        if svec[cx+dx as usize].is_digit(8) {
                            val *= mult;
                            val = val + svec[cx+dx as usize]
                                .to_digit(8)
                                    .expect("Internal error on oct convert.");
                            if 2 == dx {
                                skip += 1;
                                mult = 8;
                            }
                            else if 4 <= dx {
                                dig = false;
                            }
                            skip += 1;
                            dx += 1;
                        } else {
                            dig = false;
                        }
                    }
                    if 0 != skip {
                        output.push_str(
                            &char::from_u32(val)
                                .expect("Internal error on hex convert")
                                    .to_string()
                                       );
                    }
                    continue 'cx;
                },
                _ => (),
            }
        } else {
            output.push_str( &svec[cx].to_string().to_owned() );
        }
    }
    return (bail, output);
}

fn main() {
    let config = Config::build().unwrap_or_else(|err| {
        eprintln!("Problem parsing arguments: {err}");
        process::exit(1);
    });

    let output: String;

    if config.escapes {
        let bail: bool;
        (bail, output) = do_escape( &config.primary );
        if true == bail {
            exit( 0 );
        }
    } else {
        output = config.primary;
    }

    let arg_vec = config.rest.iter();
    print!("{}", output);
    for arg in arg_vec {
        if config.escapes {
            let bail: bool;
            let tmp: String;
            (bail, tmp) = do_escape( arg );
            print!(" {}", tmp );
            if true == bail {
                exit( 0 );
            }
        } else {
            print!(" {}", arg);
        }
    }
    if config.newline {
        print!("\n");
    }

    /* I **want** the embedded strings VERSION, LICENSE and HELP_TEXT,
     * but the optimizer kills them if it isn't POSSIBLE that they could
     * get used, this is what I came up with:
     * 1. Ask for a variable with a space in it
     *     As far as I can tell, those aren't possible.
     * 2. Ask for it's value to have very unlikely lengths.
     * Yes, these calculations waste cycles.
     */
    let neverused: String;
    match env::var("AdZd DzDa") {
        Ok(val) => neverused = val,
        Err(_) => neverused = "".to_string(),
    }
    if 12000 > neverused.len() {
        /* Kills the overhead of these three */
        if 12000 == neverused.len() {
            println!("{}", VERSION);
        }
        else if 12001 == neverused.len() {
            println!("{}", LICENSE);
        }
        else if 12002 == neverused.len() {
            println!("{}", HELP_TEXT);
        }
    }
    /* End force the optimizer into keeping my string blocks... */

    exit(0);
}
