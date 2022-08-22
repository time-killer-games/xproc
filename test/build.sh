#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Darwin" ]; then
  clang++ main.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -arch arm64 -arch x86_64;
elif [ $(uname) = "Linux" ]; then
  g++ main.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ main.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libprocstat.a /usr/lib/libutil.a /usr/lib/libelf.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static;
elif [ $(uname) = "DragonFly" ]; then
  g++ main.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static;
elif [ $(uname) = "NetBSD" ]; then
  g++ main.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static;
elif [ $(uname) = "OpenBSD" ]; then
  clang++ main.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static;
elif [ $(uname) = "SunOS" ]; then
  g++ main.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -lkvm;
else
  g++ main.cpp ../xproc.cpp -o xproc.exe -std=c++17 -Wall -static-libgcc -static-libstdc++ -static;
fi
