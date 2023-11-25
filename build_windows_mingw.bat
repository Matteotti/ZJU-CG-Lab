@echo off

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE:STRING=Release -S ./ -B build
cmake --build build --target=install

pause
