#
# Stuff the average programmer needs to change
#
set(SRCS 
	../src/renderer.cpp
	../src/popup_dlg.cpp
	../src/fnb_resources.cpp
	../src/wxFlatNotebook.cpp
)
set(HFILES 
	../include/wx/wxFlatNotebook/renderer.h
	../include/wx/wxFlatNotebook/smart_ptr.h
	../include/wx/wxFlatNotebook/singleton.h
	../include/wx/wxFlatNotebook/popup_dlg.h
	../include/wx/wxFlatNotebook/fnb_resources.h
	../include/wx/wxFlatNotebook/wxFlatNotebook.h
	../include/wx/wxFlatNotebook/wxFNBDropTarget.h
)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES}
	${PROJECT_ROOT_DIR}/wxFlatNotebook/include
)
set(LIBRARY_NAME wxFlatNotebook)
if(WIN32)
	set(CMAKE_SHARED_LINKER_FLAGS /SAFESEH:NO)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_USRDLL;/DIFLOOR_EXPORTS;/D__STDC_CONSTANT_MACROS;/DWXMAKINGDLL_FNB)
endif(WIN32)

	
if(LINUX OR APPLE)
	message("ConfigurationName" ${ConfigurationName})
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/DIFLOOR_EXPORTS;/DIFLOOR_EFFECT_PLUGIN_BASE_EXPORTS;/D__STDC_CONSTANT_MACROS)

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

add_library(${LIBRARY_NAME} SHARED ${SRCS})

set(DLL_DIR bin)
set(TARGET_LOCATION ${PROJECT_SOURCE_DIR}/${DLL_DIR}${LIB_SUFFIX})
set_target_properties(${LIBRARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TARGET_LOCATION})

target_link_libraries(${LIBRARY_NAME} ${wxWidgets_LIBRARIES})

# Precompiled header stuff must be after the target is added
set_precompiled_header(${LIBRARY_NAME} ${PROJECT_ROOT_DIR}/include/stdwx.h ${PROJECT_ROOT_DIR}/include/stdwx.cpp)
if(LINUX)
	add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${OS_BASE_NAME}${LIB_SUFFIX}/lib${LIBRARY_NAME}.so" "${PROJECT_SOURCE_DIR}/${DLL_DIR}${LIB_SUFFIX}/lib${LIBRARY_NAME}.so"
	)
endif(LINUX)
