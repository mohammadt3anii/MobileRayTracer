cmake_minimum_required( VERSION 3.6.0 )

add_subdirectory( third_party )
add_subdirectory( MobileRT )

if ( CMAKE_CXX_FLAGS MATCHES "-Wa,--noexecstack" )
    string( REGEX REPLACE "-Wa,--noexecstack" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
endif ()

if ( ANDROID_COMPILER_FLAGS MATCHES "-Wa,--noexecstack" )
    string( REGEX REPLACE "-Wa,--noexecstack" "" ANDROID_COMPILER_FLAGS ${ANDROID_COMPILER_FLAGS} )
endif ()

project( Components VERSION 1.0.0.0 LANGUAGES CXX )

file( GLOB_RECURSE MOBILE_RC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp" )
set( MOBILE_RC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}" )

set( THIRD_PARTY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/third_party" )
set( MOBILE_RT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/MobileRT/src" )

add_library( ${PROJECT_NAME} SHARED ${MOBILE_RC_SOURCES} )
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX} )

target_include_directories( ${PROJECT_NAME} PUBLIC "${THIRD_PARTY_HEADERS}" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${MOBILE_RT_HEADERS}" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${MOBILE_RC_HEADERS}" )

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )

target_compile_options( ${PROJECT_NAME} PRIVATE -std=c++14 -pedantic-errors -Weverything -Wall -Wextra -Wfatal-errors -Werror -ferror-limit=1 )
target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-c++98-compat-pedantic -Wno-padded )

target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-float-equal )

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

if( NOT DEFINED ANDROID )
    target_compile_options( ${PROJECT_NAME} PRIVATE -flto=full )
endif()

target_link_libraries( ${PROJECT_NAME}
PUBLIC tinyobjloader MobileRT PRIVATE
general -O3
general -discard-all
general -fno-rtti
general -fno-exceptions
general -flto=full
optimized -s
)
