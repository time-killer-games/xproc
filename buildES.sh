#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Linux" ]; then
  g++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -static-libgcc -static-libstdc++ -lprocps -lpthread;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -lprocstat -lutil -lc -lpthread;
elif [ $(uname) = "DragonFly" ]; then
  g++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -lkvm -lutil -lc -lpthread;
fi
