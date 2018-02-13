cmake_minimum_required( VERSION 3.6.0 )

add_subdirectory( third_party )

project( GoogleTest VERSION 1.0.0.0 LANGUAGES CXX )

file( GLOB_RECURSE GTEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

add_executable( ${PROJECT_NAME} ${GTEST_SOURCES} )
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}" )

set( MOBILE_RC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Components/src" )
set( SCENES_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Scenes" )

target_include_directories( ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include" )
target_include_directories( ${PROJECT_NAME} PUBLIC "/usr/include/libcxxabi" )

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )

target_compile_options( ${PROJECT_NAME} PRIVATE -std=c++14 -Weverything -pedantic -Wpedantic -Wall -Wextra -Wfatal-errors -Werror -ferror-limit=1 )
target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-c++98-compat-pedantic -Wno-padded )

target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-float-equal -Wno-global-constructors -Wno-weak-vtables )

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG( "-Wno-zero-as-null-pointer-constant" FLAG__WNO_ZERO_AS_NULL__SUPPORTED )

if( FLAG__WNO_ZERO_AS_NULL__SUPPORTED )
  target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-zero-as-null-pointer-constant )
endif()

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

if( NOT DEFINED ANDROID )
  target_compile_options( ${PROJECT_NAME} PRIVATE -flto=full )
endif()

target_link_libraries( ${PROJECT_NAME}
PRIVATE Components gtest
general -O3
general -discard-all
general -fno-rtti
general -fno-exceptions
general -flto=full
optimized -s
)