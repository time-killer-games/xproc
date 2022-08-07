#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Darwin" ]; then
  clang++ main.cpp apiprocess/xproc.cpp -o xproc -std=c++17 -arch arm64 -arch x86_64;
elif [ $(uname) = "Linux" ]; then
  g++ main.cpp apiprocess/xproc.cpp -o xproc -std=c++17 -static-libgcc -static-libstdc++ -lprocps;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ main.cpp apiprocess/xproc.cpp -o xproc -std=c++17 -lprocstat -lutil -lc;
elif [ $(uname) = "DragonFly" ]; then
  g++ main.cpp apiprocess/xproc.cpp -o xproc -std=c++17 -static-libgcc -static-libstdc++ -lkvm -lc;
elif [ $(uname) = "NetBSD" ]; then
  g++ main.cpp apiprocess/xproc.cpp -o xproc -std=c++17 -static-libgcc -static-libstdc++ -lkvm -lc;
elif [ $(uname) = "OpenBSD" ]; then
  clang++ main.cpp apiprocess/xproc.cpp -o xproc -std=c++17 -lkvm -lc;
else
  g++ main.cpp apiprocess/xproc.cpp -o xproc.exe -std=c++17 -static-libgcc -static-libstdc++ -static;
fi
