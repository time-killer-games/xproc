#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Darwin" ]; then
  clang++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -arch arm64 -arch x86_64;
elif [ $(uname) = "Linux" ]; then
  g++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libprocstat.a /usr/lib/libutil.a /usr/lib/libelf.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static;
elif [ $(uname) = "DragonFly" ]; then
  g++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static;
elif [ $(uname) = "NetBSD" ]; then
  g++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static;
elif [ $(uname) = "OpenBSD" ]; then
  clang++ example.cpp ../xproc.cpp /usr/lib/libkvm.a /usr/lib/libc.a -o xproc -std=c++17 -Wall -static;
elif [ $(uname) = "SunOS" ]; then
  g++ example.cpp ../xproc.cpp -o xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -lkvm;
else
  g++ example.cpp ../xproc.cpp -o xproc.exe -std=c++17 -Wall -static-libgcc -static-libstdc++ -static;
fi
