cmake_minimum_required( VERSION 3.6.0 )

add_subdirectory( third_party )
add_subdirectory( MobileRT )

project( Components VERSION 1.0 LANGUAGES CXX )

message( STATUS "Components: CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}" )

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

file( GLOB_RECURSE MOBILE_RC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp" )
set( MOBILE_RC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" )

set( THIRD_PARTY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/third_party" )
set( MOBILE_RT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/MobileRT/src" )

set( THIRD_PARTY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyobjloader/tiny_obj_loader.cc" )

message( STATUS "Components: THIRD_PARTY_HEADERS=${THIRD_PARTY_HEADERS}" )
message( STATUS "Components: MOBILE_RT_HEADERS=${MOBILE_RT_HEADERS}" )
message( STATUS "Components: MOBILE_RC_SOURCES=${MOBILE_RC_SOURCES}" )
message( STATUS "Components: MOBILE_RC_HEADERS=${MOBILE_RC_HEADERS}" )

add_library( Components SHARED ${MOBILE_RC_SOURCES} ${THIRD_PARTY_SOURCES} )
#target_link_libraries( Components PRIVATE tinyobjloader --verbose )
target_include_directories( Components PUBLIC "${THIRD_PARTY_HEADERS}" )
target_include_directories( Components PUBLIC "${MOBILE_RT_HEADERS}" )
target_include_directories( Components PUBLIC "${MOBILE_RC_HEADERS}" )

target_compile_options( Components PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )
target_compile_options( Components PRIVATE $<$<CONFIG:RELEASE>:-O3 > )

target_compile_options( Components PRIVATE -isystem /usr/include/c++/v1 )
target_compile_options( Components PRIVATE -std=c++14 -pedantic-errors -Weverything -Wall -Wextra -Wfatal-errors --verbose -Werror -ferror-limit=1 )
target_compile_options( Components PRIVATE -Wno-c++98-compat-pedantic -Wno-padded )

target_compile_options( Components PRIVATE -Wno-float-equal )

target_compile_options( Components PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

if( DEFINED NO_ANDROID )
    target_compile_definitions( Components PRIVATE -DNO_ANDROID=1 )
    target_compile_options( Components PRIVATE -flto=full )
endif( )

target_link_libraries( Components
PUBLIC MobileRT PRIVATE --verbose
general -O3
general -discard-all
general -fno-rtti
general -fno-exceptions
general -flto=full
optimized -s
)
