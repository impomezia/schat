This is a MIT-licensed [0] base32 [1] encoder/decoder implementation
in ISO C99 by Adrien Kunysz [2]. This implementation assumes 8 bits bytes
and ASCII character set.

[0] See source files for full license text or http://www.opensource.org/licenses/mit-license.php
[1] As per RFC4648: http://www.rfc-editor.org/rfc/rfc4648.txt 
[2] adrien@kunysz.be

USAGE
=====

If you just need a commandline base32 (de)coder, you can type "make" and use
the resulting "encoder" executable. It can read on stdin or from a file given
in argument. Without option, it will encode into base32. With option "-d" it
will decode from base32. Output is always on stdout.

If you need a base32 library, just integrate the base32.h and base32.c files
into your source tree. See the header file for details on how to use this
from your code.

EXAMPLES
========

Some examples are provided in the "test" target of the Makefile.

BUGS
====

* This implementation assumes 8 bits bytes and the ASCII character set.

* The Makefile has been developped for GNU Make. BSD Make and others may or
may not work.

* There is no Make target to build this as a shared library.
