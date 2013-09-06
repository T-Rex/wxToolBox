CXX := g++

WX_CONFIG   := wx-config
WX_CXXFLAGS := `$(WX_CONFIG) --cxxflags`
WX_LIBS     := `$(WX_CONFIG) --libs`

APP_NAME    := wxToolBox
LIB_FILE_NAME	:= lib$(APP_NAME).a

LIBWXTOOLBOX_FLAGS := -static -I../../include $(WX_CXXFLAGS) ../../src/wxToolBox/*.cpp

### Targets ###

all: wxToolBox

clean:
	rm -f ./*.o
	rm -f ../../lib/$(LIB_FILE_NAME)

wxToolBox:
	cd ../../src/wxToolBox/ && \
		$(CXX) -c $(LIBWXTOOLBOX_FLAGS) && ar rcs ../../lib/$(LIB_FILE_NAME) *.o && cd ../../build/