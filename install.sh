#!/bin/sh

[ winlua.c -ot winlua.exe ] || \
./build.sh && \
[ -z "${prefix}" ] && \
prefix="/usr" && \
/usr/bin/install -c -m 755 winlua.exe "${prefix}/bin"

exit ${?}
