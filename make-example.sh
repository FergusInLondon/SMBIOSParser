#!/bin/bash

# Nothing exciting in here, simple static linking to our PitParser
# Ex. http://tldp.org/HOWTO/Program-Library-HOWTO/more-examples.html

cc -Wall -g -c -o lib/libsmbios-parse.o lib/smbios_parse.c
ar rcs lib/libsmbios-parse.a lib/libsmbios-parse.o
mv lib/libsmbios-parse.a .
cc -Wall -g -c example.c -o example.o
cc -g -o example example.o -L. -lsmbios-parse  -framework IOKit -framework CoreFoundation

# Do some cleaning up
rm example.o libsmbios-parse.a lib/libsmbios-parse.o