#!/bin/sh

/usr/bin/gcc -Wall -std=gnu11 -g -O2 winlua.c -o winlua && \
/usr/bin/strip winlua

exit ${?}
