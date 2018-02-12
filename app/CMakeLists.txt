cmake_minimum_required( VERSION 3.6.0 )

add_subdirectory( Components )
add_subdirectory( Unit_Testing )

if( NOT DEFINED ANDROID )
	if( NOT CMAKE_CXX_FLAGS MATCHES ".*-fPIC.*" )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" CACHE STRING "CUSTOM" FORCE )
	endif()
endif ()

project( DrawView VERSION 1.0 LANGUAGES CXX )

file( GLOB_RECURSE SCENES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Scenes/*.cpp" )

if( NOT DEFINED ANDROID )
	find_package( GTK2 REQUIRED )
	file( GLOB_RECURSE MOBILE_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/GTK_Interface/*.cpp" )
	add_executable( ${PROJECT_NAME} ${SCENES_SOURCES} ${MOBILE_APP_SOURCES} )
	set( NO_ANDROID "1" )
elseif ( NOT DEFINED NO_ANDROID )
	file( GLOB_RECURSE MOBILE_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/JNI/*.cpp" )
	add_library( ${PROJECT_NAME} SHARED ${SCENES_SOURCES} ${MOBILE_APP_SOURCES} )
endif()

set( MOBILE_RC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Components/src" )
set( SCENES_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Scenes" )

target_include_directories( Components PUBLIC "${MOBILE_RC_HEADERS}" )
target_include_directories( Components PUBLIC "${SCENES_HEADERS}" )
target_include_directories( Components PUBLIC "${MOBILE_APP_SOURCES}" )

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )

target_compile_options( ${PROJECT_NAME} PRIVATE -std=c++14 -Weverything -pedantic -Wpedantic -Wall -Wextra -Wfatal-errors -Werror -ferror-limit=1 )
target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-c++98-compat-pedantic -Wno-padded )

target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-float-equal )

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

if( NOT DEFINED ANDROID )
  target_compile_definitions( ${PROJECT_NAME} PRIVATE -DNO_ANDROID=1 )
	target_include_directories( ${PROJECT_NAME} PRIVATE "${GTK2_INCLUDE_DIRS}" )
	target_compile_options( ${PROJECT_NAME} PRIVATE -flto=full -w )
	target_link_libraries( ${PROJECT_NAME} PRIVATE "${GTK2_LIBRARIES}" )
elseif( DEFINED ANDROID )
	target_link_libraries( ${PROJECT_NAME} PRIVATE android jnigraphics --verbose )
endif( )

target_link_libraries( ${PROJECT_NAME}
PRIVATE Components tinyobjloader --verbose
general -O3
general -discard-all
general -fno-rtti
general -fno-exceptions
general -flto=full
optimized -s
)
