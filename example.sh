#!/bin/sh
cd "${0%/*}"
gdb -batch -ex "run" -ex "bt" --args ./xproc --info-from-all 2>&1 | grep -v ^"No stack."$
