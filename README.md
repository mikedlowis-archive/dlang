DLang
==============================================

    Version:    0.3
    Created By: Michael D. Lowis
    Email:      mike@mdlowis.com

About This Project
----------------------------------------------

DLang is an experimental new programming language with arbitrarily extensible
syntax. The core of the language provides you the tools you need to get things
done while the flexible syntax allows YOU to decide how you want to get it done.
New domain specific languages can be invented on the fly with ease to solve any
problem you encounter.

License
----------------------------------------------

Unless explicitly stated otherwise, all code and documentation in this project
is released under the BSD 2-Clause License. You can find a copy of the license
text in the LICENSE.md file.

Requirements For Building
----------------------------------------------

* Ruby
* Rake (>= 0.9.2)
* Chicken Scheme Compiler
* Chicken Scheme vector-lib Egg
* A C++ compiler (Tested with gcc)

Build Instructions
----------------------------------------------

You build dlang with the following command:

    rake release

The resulting binary is placed in 'build/bin'. You can test it with the
following commands:

    build/release/bin/dlang example.dl
    ./example

Installation
----------------------------------------------

There are no installation instructions at the moment. The binary is self
contained and can be placed anywhere you want it.

Project Files and Directories
----------------------------------------------

    build/         Output directory for artifacts created during the build.
    deps/          Libraries and other dependencies the source code may have.
    docs/          Documentation for the language and the source code.
    res/           Submodules and files that aren't part of DLang source but it still needs
    source/        The source for the DLang parser
    tests/         Unit test and mock files.
    tools/         Tools required by the build system.
    Doxyfile       Doxygen documentation generator configuration.
    example.dl     An example DLang source file.
    LICENSE.md     The software license notification.
    rakefile.rb    Script containing the build configuration and tasks.
    README.md      You're reading this file right now!

Known Issues or Bugs
----------------------------------------------

This is a non-comprehensive list of known issues and bugs that I intend to fix.

* The debug build segfaults on \*nix systems due to freeing invalid pointers and other memory management issues
* Error messages are obtuse and unfriendly.
* Parser and Lexer have 0 error recovery.

Version History
----------------------------------------------

### Version 0.3

### Version 0.2

This version includes a much improved macro syntax with support for multiple
patterns similar to scheme macros. There is also a map data type and a cleaner
syntax for list literals. See example.dl for an example of the updated syntax.

### Version 0.1a

Very first version ever. Its buggy, has obtuse error messages, and supports
about 1/20 of what I would like to see. Lets call it a proof of concept.

Feature Wish List
----------------------------------------------

This is a list of features are not yet supported, but may be at some point in
the future.

* Command line options
* Multi-file support
* UTF-8 Support

More Info
----------------------------------------------

For more info or general questions shoot me an email and I'll try to get back
to you in a timely manner.

