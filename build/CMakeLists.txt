cmake_minimum_required(VERSION 2.6.0)

include(PCHSupport.cmake)
 
# Only debug & release builds
# This must be before the project command
if(WIN32 OR APPLE)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
	set(LIB_SUFFIX "") 
endif(WIN32 OR APPLE)

if(UNIX AND NOT APPLE)
	set(LINUX ON)
	set(LIB_SUFFIX /${CMAKE_BUILD_TYPE}) 
endif(UNIX AND NOT APPLE)

# Knock off ZERO_TEST at least BUILD_ALL remains
set(CMAKE_SUPPRESS_REGENERATION 1)

set(PROJECT_NAME wxToolBox)
project(${PROJECT_NAME})

# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Platform dependant variables that are cross project
# OS_BASE_NAME - Directory name for build outputs. Special third party include directories. Third party lib name suffix.
# PROJECT_ROOT_DIR - Where the root of the project starts
# THIRD_PARTY_DIR - The root directory for the third party libraries

set(wxWidgets_USE_LIBS base core adv net gl xml propgrid html aui)

if(APPLE)
	set(OS_BASE_NAME Mac)
#	set(CMAKE_OSX_SYSROOT "macosx10.5")
	set(CMAKE_CXX_FLAGS "-mmacosx-version-min=10.5" ${CMAKE_CXX_FLAGS})
endif(APPLE)
if(LINUX)
	set(OS_BASE_NAME Linux)
endif(LINUX)
if(WIN32)
	set(USE_VLD OFF)
	set(OS_BASE_NAME Win)
endif(WIN32)

set(BUILD_SHARED_LIBS 1)

find_package(wxWidgets REQUIRED)

if(wxWidgets_FOUND AND NOT DEFINED WX_VERSION_EX)
	foreach(WX_LIB ${wxWidgets_LIBRARIES})
		if(${WX_LIB} MATCHES ".*wxbase([0-9]*).*")
			set(WX_VERSION_EX ${CMAKE_MATCH_1})
		endif()
	endforeach(${W_LIB})
endif(wxWidgets_FOUND AND NOT DEFINED WX_VERSION_EX)
message("--- WX_VERSION_EX: " ${WX_VERSION_EX})

set(wxWidgets_DEFINITIONS_TEMP)
foreach(DEFINITION ${wxWidgets_DEFINITIONS})
	
	if(NOT ${DEFINITION} MATCHES "/D.*")
		set(DEFINITION "/D${DEFINITION}")
	endif()
	set(wxWidgets_DEFINITIONS_TEMP ${wxWidgets_DEFINITIONS_TEMP} 
		${DEFINITION})
endforeach(${DEFINITION})
set(wxWidgets_DEFINITIONS ${wxWidgets_DEFINITIONS_TEMP})

message("wxWidgets Definitons" ${wxWidgets_DEFINITIONS})

if(WIN32)
	SET(CPU_ARCH "x86")
	MESSAGE("Compiling for architecture: " ${CPU_ARCH})
	set(wxWidgets_DEFINITIONS ${wxWidgets_DEFINITIONS};/D_CRT_SECURE_NO_DEPRECATE;/D_CRT_NONSTDC_NO_DEPRECATE;/D_UNICODE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4996")
endif(WIN32)

set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};${wxWidgets_DEFINITIONS};/D__WXDEBUG__=1)

if(LINUX)
set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};-fPIC)
endif(LINUX)
set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)


set(BASE_INCLUDE_DIRECTORIES ${PROJECT_ROOT_DIR}/include)
if(WIN32)
	set(BASE_INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES} ${THIRD_PARTY_DIR}/GLEW/include)
endif(WIN32)

include_directories(${wxWidgets_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__WXDEBUG__=1" )
set(CMAKE_CXX_FLAGS_RELEASE "-D__WXDEBUG__=1 " )



#message("-----------------------------------------------------------")
#message("CMAKE_SHARED_LINKER_FLAGS     : ${CMAKE_SHARED_LINKER_FLAGS}")
#message("wxWidgets_DEFINITIONS     : ${wxWidgets_DEFINITIONS}")
#message("wxWidgets_FOUND           : ${wxWidgets_FOUND}")
#message("wxWidgets_INCLUDE_DIRS    : ${wxWidgets_INCLUDE_DIRS}")
#message("wxWidgets_LIBRARY_DIRS    : ${wxWidgets_LIBRARY_DIRS}")
#message("wxWidgets_LIBRARIES       : ${wxWidgets_LIBRARIES}")
#message("wxWidgets_CXX_FLAGS       : ${wxWidgets_CXX_FLAGS}")
#message("wxWidgets_USE_FILE        : ${wxWidgets_USE_FILE}")
#message("PREPROCESSOR_DEFINITIONS  : ${PREPROCESSOR_DEFINITIONS}")
#message("wxWidgets_INCLUDE_DIRS    : ${wxWidgets_INCLUDE_DIRS}")
#message("CMAKE_BUILD_TYPE    : ${CMAKE_BUILD_TYPE}")
#message("CMAKE_CXX_FLAGS_DEBUG    : ${CMAKE_CXX_FLAGS_DEBUG}")
#message("CMAKE_CXX_FLAGS_RELEASE    : ${CMAKE_CXX_FLAGS_RELEASE}")
#message("-----------------------------------------------------------")

add_subdirectory (../wxToolBox/build ../../wxToolBox/build/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../wxFlatNotebook/build ../../wxFlatNotebook/build/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../wxToolBoxMinimal ../../wxToolBoxMinimal/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../wxToolBoxSkinEditor ../../wxToolBoxSkinEditor/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../SampleIDE ../../SampleIDE/${OS_BASE_NAME}${LIB_SUFFIX})
