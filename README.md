# CMM - C Minus Minus

This is a C-- Compiler implemented in Clojure using
[instaparse](https://github.com/Engelberg/instaparse). The project will hopefully evolve to compile
elements present in the full C language, such as structs, pointers, etc.

## Usage

You can run using leiningen like so:

    $ lein run -- [options] [filenames ...]

## Options

Currently there are two options:

  --help: Displays help information
  --debug: Displays debug messages during compilation

## License

Copyright © 2015 Christopher Chapline

Distributed under the Eclipse Public License either version 1.0 or (at
your option) any later version.
