#!/bin/sh
cd "${0%/*}"
if [ $(uname) = "Darwin" ]; then
  clang++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -arch arm64 -arch x86_64; ./xproc;
  ./xproc | awk '$1 ~ /pid/; index($3, "exe") {printf "%s ",$2;sub(/[[:space:]]*([^[:space:]]+[[:space:]]+){3}/,"")}1;' | sed '/^pid/d';
elif [ $(uname) = "Linux" ]; then
  g++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./xproc;
  ./xproc | awk '$1 ~ /pid/; index($3, "exe") {printf "%s ",$2;sub(/[[:space:]]*([^[:space:]]+[[:space:]]+){3}/,"")}1;' | sed '/^pid/d';
elif [ $(uname) = "FreeBSD" ]; then
  clang++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libprocstat.a /usr/lib/libutil.a /usr/lib/libelf.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static; ./xproc;
  ./xproc | awk '$1 ~ /pid/; index($3, "exe") {printf "%s ",$2;sub(/[[:space:]]*([^[:space:]]+[[:space:]]+){3}/,"")}1;' | sed '/^pid/d';
elif [ $(uname) = "DragonFly" ]; then
  g++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./xproc;
  ./xproc | awk '$1 ~ /pid/; index($3, "exe") {printf "%s ",$2;sub(/[[:space:]]*([^[:space:]]+[[:space:]]+){3}/,"")}1;' | sed '/^pid/d';
elif [ $(uname) = "NetBSD" ]; then
  g++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./xproc;
  ./xproc | awk '$1 ~ /pid/; index($3, "exe") {printf "%s ",$2;sub(/[[:space:]]*([^[:space:]]+[[:space:]]+){3}/,"")}1;' | sed '/^pid/d';
elif [ $(uname) = "OpenBSD" ]; then
  clang++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static; ./xproc;
  ./xproc | awk '$1 ~ /pid/; index($3, "exe") {printf "%s ",$2;sub(/[[:space:]]*([^[:space:]]+[[:space:]]+){3}/,"")}1;' | sed '/^pid/d';
elif [ $(uname) = "SunOS" ]; then
  g++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -static-libgcc -lkvm -lc; ./xproc;
  ./xproc | awk '$1 ~ /pid/; index($3, "exe") {printf "%s ",$2;sub(/[[:space:]]*([^[:space:]]+[[:space:]]+){3}/,"")}1;' | sed '/^pid/d';
else
  g++ example.cpp ../xproc.cpp -o xproc.exe -std=c++17 -Wall -static-libgcc -static-libstdc++ -static -lntdll; ./xproc;
  ./xproc | awk '$1 ~ /pid/; index($3, "exe") {printf "%s ",$2;sub(/[[:space:]]*([^[:space:]]+[[:space:]]+){3}/,"")}1;' | sed '/^pid/d';
fi
