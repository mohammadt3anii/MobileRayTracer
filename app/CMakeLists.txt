cmake_minimum_required( VERSION 3.6.0 )

add_subdirectory( Components )

project( DrawView VERSION 1.0 LANGUAGES CXX )

message( STATUS "APP: CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}" )

set( CMAKE_VERBOSE_MAKEFILE on )
set( LIBCXX_FORCE_REBUILD true )
set( STLPORT_FORCE_REBUILD true )
set( LOCAL_ARM_NEON true )
set( ANDROID_ARM_NEON true )
set( ANDROID_TOOLCHAIN clang )
set( ANDROID_STL c++_shared )
set( ANDROID_ARM_MODE thumb )
set( ANDROID_DISABLE_NO_EXECUTE true )
set( ANDROID_DISABLE_FORMAT_STRING_CHECKS true )
set( ANDROID_PIE on )

file( GLOB_RECURSE SCENES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Scenes/*.cpp" )

if( DEFINED NO_ANDROID )
	find_package( GTK2 REQUIRED )
	file( GLOB_RECURSE MOBILE_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/GTK_Interface/*.cpp" )
	add_executable( DrawView ${SCENES_SOURCES} ${MOBILE_APP_SOURCES} )
elseif ( NOT DEFINED NO_ANDROID )
    file( GLOB_RECURSE MOBILE_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/JNI/*.cpp" )
    add_library( DrawView SHARED ${SCENES_SOURCES} ${MOBILE_APP_SOURCES} )
endif( )

set( MOBILE_RC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Components/src" )
set( SCENES_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Scenes" )

target_include_directories( Components PUBLIC "${MOBILE_RC_HEADERS}" )
target_include_directories( Components PUBLIC "${SCENES_HEADERS}" )
target_include_directories( Components PUBLIC "${MOBILE_APP_SOURCES}" )

target_compile_options( DrawView PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )
target_compile_options( DrawView PRIVATE $<$<CONFIG:RELEASE>:-O3 -DNDEBUG> )

target_compile_options( DrawView PRIVATE -isystem /usr/include/c++/v1 )
target_compile_options( DrawView PRIVATE -std=c++14 -Weverything -pedantic -Wpedantic -Wall -Wextra -Wfatal-errors -Werror -ferror-limit=1 )
target_compile_options( DrawView PRIVATE -Wno-c++98-compat-pedantic -Wno-padded )

target_compile_options( DrawView PRIVATE -Wno-float-equal )

target_compile_options( DrawView PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

if( DEFINED NO_ANDROID )
    target_compile_definitions( DrawView PRIVATE -DNO_ANDROID=1 )
	target_include_directories( DrawView PRIVATE "${GTK2_INCLUDE_DIRS}" )
	target_compile_options( DrawView PRIVATE -flto=full -w )
	target_link_libraries( DrawView PRIVATE "${GTK2_LIBRARIES}" )
elseif( NOT DEFINED NO_ANDROID )
	target_link_libraries( DrawView PRIVATE android jnigraphics --verbose )
endif( )

target_link_libraries( DrawView
PRIVATE Components --verbose
general -O3
general -discard-all
general -fno-rtti
general -fno-exceptions
general -flto=full
optimized -s
)
