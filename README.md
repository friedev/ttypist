# ttypist

A terminal-based typing test written in C using ncurses, with a wrapper script written in POSIX shell.

## Dependencies

- C
- ncurses
- GNU make
- POSIX-compliant shell

## Compiling

```sh
make
```

## Installation

As root:

```sh
make install
```

## Usage

To use a custom string for the typing test:

```sh
ttypist "my custom string"
```

To generate a random string of 10 words from a word list:

```sh
ttypist-gen /path/to/words.txt 10
```

## Word Lists

A word list is a plain text file consisting of individual words each separated by a newline (LF).

If you have the UNIX dictionary installed, you can use `/usr/share/dict/words`.
(On Arch Linux, install the `words` package.)

Other word lists:

- [ttyper](https://github.com/max-niederman/ttyper/tree/main/resources/runtime/language)
- [tt](https://github.com/lemnos/tt/tree/master/words)

You can also search for word lists online (e.g. "english words txt").
Make sure to convert to LF line endings instead of CRLF if necessary (e.g. use `dos2unix`).

## License

ttypist is licensed under the [MIT License](https://mit-license.org/).

## Similar Programs

- [ttyper](https://github.com/max-niederman/ttyper) (Rust)
- [tt](https://github.com/lemnos/tt) (Go)
- [typespeed](http://typespeed.sourceforge.net) (C)
