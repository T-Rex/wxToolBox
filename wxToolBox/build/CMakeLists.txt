#
# Stuff the average programmer needs to change
#
set(SRCS 
	../src/wxToolBox.cpp
	../src/wxToolObject.cpp
	../src/wxToolBoxTab.cpp
	../src/wxToolBoxItem.cpp
	../src/wxToolBoxEvent.cpp
	../src/wxToolBoxScrollButton.cpp
	../src/wxToolObjectBackground.cpp
	../src/wxToolItemEditorTextCtrl.cpp
)
set(HFILES
	../include/wx/wxToolBox/wxToolBox.h
	../include/wx/wxToolBox/wxToolBox_MainPage.h
)

set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
	${PROJECT_ROOT_DIR}/wxToolBox/include
)
set(LIBRARY_NAME wxToolBox)
if(WIN32)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/D_WIN32_WINNT=0x0501;/DWXTOOLBOX_EXPORTS)
endif(WIN32)

if(LINUX OR APPLE)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/DWXTOOLBOX_EXPORTS)
	
	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        if(LINUX)
                SET(CMAKE_INSTALL_RPATH ".:./../../")
        else()
                SET(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
        endif(LINUX)
endif(LINUX OR APPLE)

set(SRCS ${SRCS} ${HFILES} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})
link_directories(${LINK_DIRECTORIES})

add_library(${LIBRARY_NAME} SHARED ${SRCS})

set(DLL_DIR bin)
set(TARGET_LOCATION ${PROJECT_SOURCE_DIR}/${DLL_DIR}${LIB_SUFFIX})

if(APPLE)
	set(TARGET_LOCATION 
		${TARGET_LOCATION}/$(CONFIGURATION)/${PROJECT_NAME}.app/Contents/Frameworks)
endif(APPLE)
if(LINUX OR APPLE)
	get_target_property(RESULT_FULL_PATH ${LIBRARY_NAME} LOCATION)
	get_filename_component(RESULT_FILE_NAME ${RESULT_FULL_PATH} NAME)
endif(LINUX OR APPLE)

set_target_properties(${LIBRARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TARGET_LOCATION})

target_link_libraries(${LIBRARY_NAME} ${wxWidgets_LIBRARIES})

# Precompiled header stuff must be after the target is added
set_precompiled_header(${LIBRARY_NAME} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)
if(LINUX)
	add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${OS_BASE_NAME}${LIB_SUFFIX}/lib${LIBRARY_NAME}.so" "${PROJECT_SOURCE_DIR}/${DLL_DIR}${LIB_SUFFIX}/lib${LIBRARY_NAME}.so"
	)
endif(LINUX)

if(LINUX OR APPLE)
	add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIBRARY_NAME}> ${TARGET_LOCATION}/${RESULT_FILE_NAME}
	)
endif(LINUX OR APPLE)
