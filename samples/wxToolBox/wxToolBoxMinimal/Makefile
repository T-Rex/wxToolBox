# Project: wxToolBoxTest
# Compiler: Default GCC compiler
# Compiler Type: MingW 3
# Makefile created by wxDev-C++ 6.10.2 on 04.04.07 00:50

CPP       = g++.exe
CC        = gcc.exe
WINDRES   = windres.exe
OBJ       = wxToolBoxMinimal.o
LINKOBJ   = wxToolBoxMinimal.o
LIBS      = -L"../../../lib" -L/usr/local/lib -lwxToolBox `wx-config --libs` 

INCS      = -I"include" -I"./" -I"../../../include"
CXXINCS   = `wx-config --cxxflags` -I"./" -I"../../../include"
BIN       = ../bin/wxToolBoxTest_DevCPP.exe
DEFINES   = 
CXXFLAGS  = $(CXXINCS) $(DEFINES) -fno-rtti -fno-exceptions -fno-pcc-struct-return -fstrict-aliasing -Wall -D__WXMSW__ -D__GNUWIN32__ -D__WIN95__   -fexpensive-optimizations -O3
CFLAGS    = $(INCS) $(DEFINES) -fno-rtti -fno-exceptions -fno-pcc-struct-return -fstrict-aliasing -Wall -fvtable-thunks -D__WXMSW__ -D__GNUWIN32__ -D__WIN95__   -fexpensive-optimizations -O3
GPROF     = gprof.exe
RM        = rm -f
LINK      = g++.exe

.PHONY: all all-before all-after clean clean-custom
all: all-before $(BIN) all-after

clean: clean-custom
	$(RM) $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(LINK) $(LINKOBJ) -o "..\bin\wxToolBoxTest_DevCPP.exe" $(LIBS)

wxToolBoxMinimal.o: $(GLOBALDEPS) wxToolBoxMinimal.cpp
	$(CPP) -c wxToolBoxMinimal.cpp -o wxToolBoxMinimal.o $(CXXFLAGS)
