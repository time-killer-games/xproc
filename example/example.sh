#!/bin/sh
cd "${0%/*}"
if [ $(uname) = "Darwin" ]; then
  clang++ example.cpp ../process.cpp -o ../xproc -std=c++17 -Wall -arch arm64 -arch x86_64; ../xproc;
elif [ $(uname) = "Linux" ]; then
  g++ example.cpp ../process.cpp -o ../xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ../xproc;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ example.cpp ../process.cpp -o ../xproc -std=c++17 -Wall -lelf -lkvm -lc -lpthread -static; ../xproc;
elif [ $(uname) = "DragonFly" ]; then
  g++ example.cpp ../process.cpp -o ../xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -lkvm -lc -lpthread -static; ../xproc;
elif [ $(uname) = "NetBSD" ]; then
  g++ example.cpp ../process.cpp -o ../xproc -std=c++17 -Wall -static-libgcc -static-libstdc++ -lkvm -lc -lpthread -static; ../xproc;
elif [ $(uname) = "OpenBSD" ]; then
  clang++ example.cpp ../process.cpp -o ../xproc -std=c++17 -Wall -lkvm -lc -lpthread -static; ../xproc;
elif [ $(uname) = "SunOS" ]; then
  g++ example.cpp ../process.cpp -o ../xproc -std=c++17 -Wall -static-libgcc -lkvm; ../xproc;
else
  g++ example.cpp ../process.cpp -o ../xproc.exe -std=c++17 -Wall -static-libgcc -static-libstdc++ -static -lntdll; ../xproc;
fi
