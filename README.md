# CMM - C Minus Minus

This is a C-- Compiler implemented in Clojure using
[instaparse](https://github.com/Engelberg/instaparse). The project will hopefully evolve to compile
elements present in the full C language, such as structs, pointers, etc.

## Usage

You can run using leiningen like so:

    $ lein run -- [options] [filenames ...]

Additionally, you can build a jar file by running:

  $ lein uberjar

Which can be run by invoking Java:

  $ java -jar cmm.jar [options] [filenames ...]

## Options

Currently there are two options:

```
  --help: Displays help information
  --debug: Displays debug messages during compilation
```

## License

Copyright © 2016 Christopher Chapline

Distributed under the Eclipse Public License either version 1.0 or (at
your option) any later version.

