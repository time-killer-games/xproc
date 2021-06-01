#!/bin/sh
cd "${0%/*}"
sleep 2
echo "Run: ./xproc --help"
sleep 1
./xproc --help
sleep 2
echo "Run: ./xproc --pid-enum"
sleep 1
./xproc --pid-enum
sleep 2
echo "Run: ./xproc --pid-enum | xargs -L1 ./xproc --ppid-from-pid"
sleep 1
./xproc --pid-enum | xargs -L1 ./xproc --ppid-from-pid
sleep 2
echo "Run: ./xproc --pid-enum | xargs -L1 ./xproc --pid-from-ppid"
sleep 1
./xproc --pid-enum | xargs -L1 ./xproc --pid-from-ppid
sleep 2
echo "Run: ./xproc --pid-enum | xargs -L1 ./xproc --exe-from-pid"
sleep 1
./xproc --pid-enum | xargs -L1 ./xproc --exe-from-pid
sleep 2
echo "Run: ./xproc --pid-enum | xargs -L1 ./xproc --cwd-from-pid"
sleep 1
./xproc --pid-enum | xargs -L1 ./xproc --cwd-from-pid
sleep 2
echo "Run: ./xproc --pid-enum | xargs -L1 ./xproc --cmd-from-pid"
sleep 1
./xproc --pid-enum | xargs -L1 ./xproc --cmd-from-pid
sleep 2
echo "Run: ./xproc --pid-enum | xargs -L1 ./xproc --env-from-pid"
sleep 1
./xproc --pid-enum | xargs -L1 ./xproc --env-from-pid
sleep 2
echo "Run: ./xproc --pid-enum | xargs -L1 ./xproc --wid-from-pid"
sleep 1
./xproc --pid-enum | xargs -L1 ./xproc --wid-from-pid
sleep 2