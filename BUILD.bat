@echo off
setlocal EnableDelayedExpansion

if not exist Temp\ (
    mkdir Temp
)

cd Temp

cmake -G "MSYS Makefiles" ../
make && make install

cd ..