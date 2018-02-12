cmake_minimum_required( VERSION 3.6.0 )

project( MobileRT VERSION 1.0 LANGUAGES CXX )

message( STATUS "MobileRT: CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}" )

get_cmake_property( _variableNames VARIABLES )
foreach ( _variableName ${_variableNames} )
    #message( STATUS "${_variableName}=${${_variableName}}" )
endforeach()
#message( FATAL_ERROR "ACABOU" )

#string( REGEX REPLACE "-Wa,--noexecstack" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
#string( REGEX REPLACE "-Os" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} )

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

file( GLOB_RECURSE MOBILE_RT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp" )
set( MOBILE_RT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src" )

add_library( MobileRT SHARED ${MOBILE_RT_SOURCES} )
target_include_directories( MobileRT PUBLIC "${MOBILE_RT_HEADERS}" )

target_compile_options( MobileRT PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )
target_compile_options( MobileRT PRIVATE $<$<CONFIG:RELEASE>:-O3 > )

target_compile_options( MobileRT PRIVATE -isystem /usr/include/c++/v1 )
target_compile_options( MobileRT PRIVATE -std=c++14 -Weverything -Wall -Wextra -Wfatal-errors -Werror -ferror-limit=1 )
target_compile_options( MobileRT PRIVATE -Wno-c++98-compat-pedantic -Wno-padded )

target_compile_options( MobileRT PRIVATE -Wno-float-equal )

target_compile_options( MobileRT PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )


if( DEFINED NO_ANDROID )
    target_compile_definitions( MobileRT PRIVATE -DNO_ANDROID=1 )
    target_compile_options( MobileRT PRIVATE -flto=full )
    target_link_libraries( MobileRT PUBLIC c++ pthread )
else( NOT DEFINED NO_ANDROID )
    target_link_libraries( MobileRT PUBLIC log )
endif( )

target_link_libraries( MobileRT
PUBLIC atomic PRIVATE --verbose
general -O3
general -discard-all
general -fno-rtti
general -fno-exceptions
general -flto=full
optimized -s
)
