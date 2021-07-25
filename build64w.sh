#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Darwin" ]; then
  clang++ main.cpp crossprocess.cpp -o xproc -std=c++17 -DXPROCESS_GUIWINDOW_IMPL -framework CoreFoundation  -framework CoreGraphics -arch arm64 -arch x86_64;
elif [ $(uname) = "Linux" ]; then
  g++ main.cpp crossprocess.cpp -o xproc -std=c++17 -static-libgcc -static-libstdc++ -lprocps -lpthread -DXPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs` -m64;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ main.cpp crossprocess.cpp -o xproc -std=c++17 -lprocstat -lutil -lc -lpthread -DXPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs` -m64;
elif [ $(uname) = "DragonFly" ]; then
  g++ main.cpp crossprocess.cpp -o xproc -std=c++17 -lkvm -lutil -lc -lpthread -DXPROCESS_GUIWINDOW_IMPL `pkg-config x11 --cflags --libs` -m64;
else
  C:/msys64/msys2_shell.cmd -defterm -mingw32 -no-start -here -lc "g++ crossprocess.cpp -o crossprocess32.exe -std=c++17 -static-libgcc -static-libstdc++ -static -m32";
  C:/msys64/msys2_shell.cmd -defterm -mingw64 -no-start -here -lc "g++ crossprocess.cpp -o crossprocess64.exe -std=c++17 -static-libgcc -static-libstdc++ -static -m64";
  xxd -i 'crossprocess32' | sed 's/\([0-9a-f]\)$/\0, 0x00/' > 'crossprocess32.h'
  xxd -i 'crossprocess64' | sed 's/\([0-9a-f]\)$/\0, 0x00/' > 'crossprocess64.h'
  rm -f "crossprocess32.exe" "crossprocess64.exe"
  C:/msys64/msys2_shell.cmd -defterm -mingw64 -no-start -here -lc "g++ main.cpp crossprocess.cpp -o xproc.exe -std=c++17 -static-libgcc -static-libstdc++ -static -lShlwapi -DXPROCESS_WIN32EXE_INCLUDES -DXPROCESS_GUIWINDOW_IMPL -m64";
  rm -f "crossprocess32.h" "crossprocess64.h"
fi
