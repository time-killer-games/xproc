#!/bin/sh
cd "${0%/*}"
gdb ./xproc 
run --info-from-all
bt full
