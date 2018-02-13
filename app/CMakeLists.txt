cmake_minimum_required( VERSION 3.6.0 )

string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_L )
if( CMAKE_BUILD_TYPE_L MATCHES debug )
	if( DEFINED ANDROID )
		set( CMAKE_DEBUG_POSTFIX "" )
	else()
		set( CMAKE_DEBUG_POSTFIX "d" )
	endif()
endif()

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( THIRD_PARTY_HEADERS "${CMAKE_SOURCE_DIR}/third_party" )

add_subdirectory( Components )
add_subdirectory( Unit_Testing )
add_subdirectory( third_party )

if( NOT DEFINED ANDROID )
	string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_CXX_FLAGS_L )
	if( NOT CMAKE_CXX_FLAGS_L MATCHES ".*-fpic.*" )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" CACHE STRING "CUSTOM" FORCE )
	endif()
endif ()

project( AppInterface VERSION 1.0.0.0 LANGUAGES CXX )

file( GLOB_RECURSE SCENES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Scenes/*.cpp" )

if( DEFINED ANDROID )
	file( GLOB_RECURSE MOBILE_JNI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/JNI/*.cpp" )
	add_library( ${PROJECT_NAME} SHARED ${SCENES_SOURCES} ${MOBILE_JNI_SOURCES} )
else ()
	find_package( GTK2 REQUIRED )
	file( GLOB_RECURSE MOBILE_GTK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/GTK_Interface/*.cpp" )
	add_executable( ${PROJECT_NAME} ${SCENES_SOURCES} ${MOBILE_GTK_SOURCES} )
endif()

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}" )

set( MOBILE_RC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Components/src" )
set( SCENES_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Scenes" )
set( GSL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/GSL/include" )

target_include_directories( ${PROJECT_NAME} PUBLIC "${MOBILE_RC_HEADERS}" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${SCENES_HEADERS}" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${MOBILE_APP_SOURCES}" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${GSL_HEADERS}" )

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )

target_compile_options( ${PROJECT_NAME} PRIVATE -std=c++14 -Weverything -pedantic -Wpedantic -Wall -Wextra -Wfatal-errors -Werror -ferror-limit=1 )
target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-c++98-compat-pedantic -Wno-padded )

target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-float-equal )

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

if( DEFINED ANDROID )
	target_compile_definitions( ${PROJECT_NAME} PRIVATE -DANDROID=1 )
	target_link_libraries( ${PROJECT_NAME} PRIVATE android jnigraphics )
else()
	target_include_directories( ${PROJECT_NAME} PRIVATE "${GTK2_INCLUDE_DIRS}" )
	target_compile_options( ${PROJECT_NAME} PRIVATE -flto=full -w )
	target_link_libraries( ${PROJECT_NAME} PRIVATE "${GTK2_LIBRARIES}" )
endif()

target_link_libraries( ${PROJECT_NAME}
PRIVATE Components tinyobjloader GSL
general -O3
general -discard-all
general -fno-rtti
general -fno-exceptions
general -flto=full
optimized -s
)