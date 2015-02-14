set(SRCS
	ToolBoxHostFrame.cpp
	ToolBoxTestFrame.cpp
	wxAdvImageFileProperty.cpp
	wxBitmapSetInfoProperty.cpp
	wxToolBoxSkinEditorApp.cpp
	wxToolObjectBackgroundInfoProperty.cpp)
	
set(HEADERS
	AdditionalProperties.h
	ToolBoxHostFrame.h
	ToolBoxTestFrame.h
	wxToolBoxSkinEditorApp.h
	wxToolBoxSkinEditorHeaders.h)

set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
	${PROJECT_ROOT_DIR}/wxToolBox/include
	${PROJECT_ROOT_DIR}/wxFlatNotebook/include)

if(WIN32)
	set(SRCS ${SRCS} wxToolBoxSkinEditor.rc)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};
		/D_USRDLL;
		/DwxUSE_NO_MANIFEST=1;
		/D__STDC_CONSTANT_MACROS)
	set(LINK_DIRECTORIES 
		${PROJECT_ROOT_DIR}/wxToolBox/build/${OS_BASE_NAME}${LIB_SUFFIX}/$(ConfigurationName)
		${PROJECT_ROOT_DIR}/wxFlatNotebook/build/${OS_BASE_NAME}${LIB_SUFFIX}/$(ConfigurationName))
	set(WXTOOLBOX_LIBS wxToolBox.lib wxFlatNotebook.lib)
endif(WIN32)
if(LINUX OR APPLE)
	set(LINK_DIRECTORIES 
		${PROJECT_ROOT_DIR}/wxToolBox/build/${OS_BASE_NAME}${LIB_SUFFIX}
		${PROJECT_ROOT_DIR}/wxFlatNotebook/build/${OS_BASE_NAME}${LIB_SUFFIX})
	set(WXTOOLBOX_LIBS wxToolBox wxFlatNotebook)
	set(WXTOOLBOX_LIBS_SHARED wxToolBox wxFlatNotebook)
endif(LINUX OR APPLE)

set(LIBS ${WXTOOLBOX_LIBS} ${wxWidgets_LIBRARIES})

set(EXECUTABLE_NAME wxToolBoxSkinEditor)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES})
link_directories(${LINK_DIRECTORIES})

if(WIN32)
	set(EXECUTABLE_TYPE WIN32)
endif(WIN32)
if(APPLE)
	set(MACOSX_BUNDLE YES)
	set(EXECUTABLE_TYPE MACOSX_BUNDLE)
	set(CMAKE_INSTALL_PATH "@loader_path")
endif(APPLE)
if(LINUX)
	set(EXECUTABLE_TYPE "")
endif(LINUX)

set(SRCS ${SRCS} ${HEADERS} 
	${PROJECT_ROOT_DIR}/include/stdwx.h 
	${PROJECT_ROOT_DIR}/include/stdwx.cpp)

set(PROJECT_FILES ${SRCS} ${HEADERS})
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_TYPE} ${PROJECT_FILES})

set(EXE_DIR bin)
set(TARGET_LOCATION ${PROJECT_SOURCE_DIR}/${EXE_DIR}${LIB_SUFFIX})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TARGET_LOCATION})
if(APPLE)
	set(CMAKE_MACOSX_RPATH 1)
	set(MACOSX_RPATH TRUE)
        set_target_properties(
                ${EXECUTABLE_NAME} PROPERTIES 
                INSTALL_RPATH "@loader_path/../Frameworks")
endif(APPLE)

target_link_libraries(${EXECUTABLE_NAME} ${LIBS})

add_dependencies(${EXECUTABLE_NAME} wxToolBox wxFlatNotebook)

set_precompiled_header(${EXECUTABLE_NAME} 
	${PROJECT_ROOT_DIR}/include/stdwx.h 
	${PROJECT_ROOT_DIR}/include/stdwx.cpp)

if(APPLE)
	FOREACH(DEP_LIB ${WXTOOLBOX_LIBS_SHARED})
		get_filename_component(ABS_ROOT_DIR ${PROJECT_ROOT_DIR} ABSOLUTE)
		set(LIBNAME_FULL "${ABS_ROOT_DIR}/${DEP_LIB}/${OS_BASE_NAME}${LIB_SUFFIX}/$(CONFIGURATION)/lib${DEP_LIB}.dylib")
                add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                	COMMAND install_name_tool -change "${LIBNAME_FULL}" "@executable_path/../Frameworks/lib${DEP_LIB}.dylib" $<TARGET_FILE:${EXECUTABLE_NAME}>)
        ENDFOREACH(DEP_LIB)
endif(APPLE)

add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/Skins $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/graphics $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
					  