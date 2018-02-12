cmake_minimum_required( VERSION 3.6.0 )

add_subdirectory( third_party )

project( GoogleTest VERSION 1.0 LANGUAGES CXX )

file( GLOB_RECURSE GTEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

add_executable( GoogleTest ${GTEST_SOURCES} )

set( MOBILE_RC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Components/src" )
set( SCENES_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Scenes" )

target_include_directories( GoogleTest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include" )
target_include_directories( GoogleTest PUBLIC "/usr/include/libcxxabi" )

target_compile_options( GoogleTest PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )

target_compile_options( GoogleTest PRIVATE -std=c++14 -Weverything -pedantic -Wpedantic -Wall -Wextra -Wfatal-errors -Werror -ferror-limit=1 )
target_compile_options( GoogleTest PRIVATE -Wno-c++98-compat-pedantic -Wno-padded )

target_compile_options( GoogleTest PRIVATE -Wno-float-equal )

target_compile_options( GoogleTest PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

target_compile_options( GoogleTest PRIVATE -flto=full -w )
target_link_libraries( GoogleTest PRIVATE gtest_main gtest gmock gmock_main )

if( NOT DEFINED ANDROID )
    target_compile_definitions( GoogleTest PRIVATE -DNO_ANDROID=1 )
endif( )

target_link_libraries( GoogleTest
PRIVATE Components --verbose
general -O3
general -discard-all
general -fno-rtti
general -fno-exceptions
general -flto=full
optimized -s
)