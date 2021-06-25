#!/bin/sh
cd "${0%/*}"
gdb --args ./xproc --info-from-all
