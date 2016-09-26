#!/bin/sh

/usr/bin/gcc -Wall -std=gnu11 -g -O2 winlua.c -o /usr/bin/winlua && \
/usr/bin/strip /usr/bin/winlua

exit ${?}
