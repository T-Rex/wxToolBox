#!/bin/bash
echo OS Type: $OSTYPE

# ----------------------------------
# build Debug configuration makefile
# ----------------------------------
echo building Debug configuration makefile
echo directory "LinuxDebug"
rm -dr "LinuxDebug"
mkdir "LinuxDebug"
cd "LinuxDebug"
cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Debug ../ 
#	cmake -DCMAKE_BUILD_TYPE:STRING=Debug ../ 
cd ..

# ----------------------------------
# build Release configuration makefile
# ----------------------------------
echo building Release configuration makefile
echo directory "LinuxRelease"
rm -dr "LinuxRelease"
mkdir "LinuxRelease"
cd "LinuxRelease"
cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Release ../ 
#	cmake -DCMAKE_BUILD_TYPE:STRING=Release ../ 
cd ..

