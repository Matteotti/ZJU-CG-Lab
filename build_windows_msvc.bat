@echo off

cmake -S ./ -B build
cmake --build build --target=install --config Release

pause
