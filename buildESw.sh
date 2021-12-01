#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Linux" ]; then
  g++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -static-libgcc -static-libstdc++ -lprocps -lpthread -DPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs`;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -lprocstat -lutil -lc -lpthread -DPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs`;
elif [ $(uname) = "DragonFly" ]; then
  g++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -lkvm -lutil -lc -lpthread -DPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs`;
fi
