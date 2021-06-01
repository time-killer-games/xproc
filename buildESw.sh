#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Linux" ]; then
  g++ main.cpp crossprocess.cpp -o xproc -std=c++17 -static-libgcc -static-libstdc++ -lprocps -lpthread -DXPROCESS_GUIWINDOW_IMPL `shell pkg-config x11 --cflags --libs`;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ main.cpp crossprocess.cpp -o xproc -std=c++17 -lprocstat -lutil -lc -lpthread -DXPROCESS_GUIWINDOW_IMPL `shell pkg-config x11 --cflags --libs`;
fi
