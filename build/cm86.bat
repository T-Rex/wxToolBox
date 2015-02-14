rem @echo off
rmdir /S /Q Win
mkdir Win
cd Win
cmake ../ -G "Visual Studio 12"
cd ..
