# Hamming Codes

-----------------------
DESCRIPTION
Two programs, encode and decode, will encode and decode a series of inputs as well as provide statistics on the result.

-----------------------
FILES

-
error.c

This file contains the program code to inject noise into an input. Written by Eugene Chou.

-
entropy.c

This file contains the program code to calculate the entropy of an input. Written by Eugene Chou.

-
encode.c

This file contains the program code to encode an input

-
decode.c

This file contains the program code to decode an input

-
bv.c/h

This file contains the code needed to create the BitVector ADT.

-
bm.c/h

This file contains the code needed to create the BitMatrix ADT.

-
hamming.c/h

This file contains code for functions that encode and decode inputs.

-
Makefile

This file contains instructions needed to compile the code

-
README.md

This file contains information about the instructions and files needed to run the program.

-----------------------
INSTRUCTIONS

Ensure that all files are present. Type make in the terminal to create several executable files, namely "encode", "decode", "error", and "entropy". 
To understand the various command line options, type "./[executable] -h ". 
An example command will show with the proper formatting and short explanations of each command line option.
"-h" will print the help message, "-i [file]" will specify the file to take as the input with stdin as default. 
The command "-o [file]" will specify the file to write to as the output with stdout as the default, and "-v" will print out statistics for the file decoded.
