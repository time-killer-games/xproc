#!/bin/sh
cd "${0%/*}"
if [ $(uname) = "Darwin" ]; then
  clang++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -arch arm64 -arch x86_64; ./xproc;
  ./xproc | awk '{if ($3 ~ "exe") {printf $2;{for (i=4; i<=NF; i++) if (i==NF) {print $i;} else {printf "%s ", $i;}}}}'
elif [ $(uname) = "Linux" ]; then
  g++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./xproc;
  ./xproc | awk '{if ($3 ~ "exe") {printf $2;{for (i=4; i<=NF; i++) if (i==NF) {print $i;} else {printf "%s ", $i;}}}}'
elif [ $(uname) = "FreeBSD" ]; then
  clang++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libprocstat.a /usr/lib/libutil.a /usr/lib/libelf.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static; ./xproc;
  ./xproc | awk '{if ($3 ~ "exe") {printf $2;{for (i=4; i<=NF; i++) if (i==NF) {print $i;} else {printf "%s ", $i;}}}}'
elif [ $(uname) = "DragonFly" ]; then
  g++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./xproc;
  ./xproc | awk '{if ($3 ~ "exe") {printf $2;{for (i=4; i<=NF; i++) if (i==NF) {print $i;} else {printf "%s ", $i;}}}}'
elif [ $(uname) = "NetBSD" ]; then
  g++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./xproc;
  ./xproc | awk '{if ($3 ~ "exe") {printf $2;{for (i=4; i<=NF; i++) if (i==NF) {print $i;} else {printf "%s ", $i;}}}}'
elif [ $(uname) = "OpenBSD" ]; then
  clang++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static; ./xproc;
  ./xproc | awk '{if ($3 ~ "exe") {printf $2;{for (i=4; i<=NF; i++) if (i==NF) {print $i;} else {printf "%s ", $i;}}}}'
elif [ $(uname) = "SunOS" ]; then
  g++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -static-libgcc -lkvm -lc; ./xproc;
  ./xproc | awk '{if ($3 ~ "exe") {printf $2;{for (i=4; i<=NF; i++) if (i==NF) {print $i;} else {printf "%s ", $i;}}}}'
else
  g++ example.cpp ../xproc.cpp -o xproc.exe -std=c++17 -Wall -static-libgcc -static-libstdc++ -static -lntdll; ./xproc.exe;
  ./xproc.exe | awk '{if ($3 ~ "exe") {printf $2;{for (i=4; i<=NF; i++) if (i==NF) {print $i;} else {printf "%s ", $i;}}}}'
fi
