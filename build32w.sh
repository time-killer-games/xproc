#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Darwin" ]; then
  clang++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -DPROCESS_GUIWINDOW_IMPL -framework CoreFoundation -framework CoreGraphics -m32;
elif [ $(uname) = "Linux" ]; then
  g++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -static-libgcc -static-libstdc++ -lprocps -lpthread -DPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs` -m32;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -lprocstat -lutil -lc -lpthread -DPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs` -m32;
elif [ $(uname) = "DragonFly" ]; then
  g++ main.cpp apiprocess/process.cpp -o xproc -std=c++17 -lkvm -lutil -lc -lpthread -DPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs` -m32;
else
  C:/msys64/msys2_shell.cmd -defterm -mingw32 -no-start -here -lc "g++ apiprocess/process.cpp -o process32.exe -std=c++17 -static-libgcc -static-libstdc++ -static -m32";
  C:/msys64/msys2_shell.cmd -defterm -mingw64 -no-start -here -lc "g++ apiprocess/process.cpp -o process64.exe -std=c++17 -static-libgcc -static-libstdc++ -static -m64";
  xxd -i 'process32' | sed 's/\([0-9a-f]\)$/\0, 0x00/' > 'apiprocess/process32.h'
  xxd -i 'process64' | sed 's/\([0-9a-f]\)$/\0, 0x00/' > 'apiprocess/process64.h'
  rm -f "process32.exe" "process64.exe"
  C:/msys64/msys2_shell.cmd -defterm -mingw32 -no-start -here -lc "g++ main.cpp apiprocess/process.cpp -o xproc.exe -std=c++17 -static-libgcc -static-libstdc++ -static -DPROCESS_WIN32EXE_INCLUDES -DPROCESS_GUIWINDOW_IMPL -m32";
  rm -f "apiprocess/process32.h" "apiprocess/process64.h"
fi
