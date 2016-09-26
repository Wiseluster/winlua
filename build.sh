#!/bin/sh

/usr/bin/gcc -Wall -std=gnu11 -g -O2 winlua.c -o winlua.exe && \
/usr/bin/strip winlua.exe

exit ${?}
