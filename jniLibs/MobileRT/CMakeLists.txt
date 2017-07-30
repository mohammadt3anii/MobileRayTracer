cmake_minimum_required( VERSION 3.6.0 )

get_cmake_property( _variableNames VARIABLES )
foreach ( _variableName ${_variableNames} )
    #message( STATUS "${_variableName}=${${_variableName}}" )
endforeach()
#message( FATAL_ERROR "ACABOU" )

STRING( REGEX REPLACE "-Wa,--noexecstack" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
STRING( REGEX REPLACE "-Os" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} )

set( CMAKE_VERBOSE_MAKEFILE on )
set( LIBCXX_FORCE_REBUILD FALSE )
set( STLPORT_FORCE_REBUILD FALSE )
set( LOCAL_ARM_NEON TRUE )
set( ANDROID_ARM_NEON TRUE )
set( ANDROID_TOOLCHAIN clang )
set( ANDROID_STL c++_shared )
set( ANDROID_ARM_MODE THUMB )
set( ANDROID_DISABLE_NO_EXECUTE TRUE )
set( ANDROID_DISABLE_FORMAT_STRING_CHECKS TRUE )
set( ANDROID_PIE ON )

file( GLOB_RECURSE MOBILE_RT_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/**.cpp" )
add_library( MobileRT SHARED ${MOBILE_RT_SOURCES} )
target_include_directories( MobileRT INTERFACE "${CMAKE_CURRENT_LIST_DIR}/src/main/cpp" )

target_compile_options( MobileRT PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )
target_compile_options( MobileRT PRIVATE $<$<CONFIG:RELEASE>:-Ofast> )

target_compile_options( MobileRT PRIVATE -std=c++14 -Weverything -Wall -Wfatal-errors --verbose -Werror -ferror-limit=1 -gsplit-dwarf )
target_compile_options( MobileRT PRIVATE -Wno-c++98-compat-pedantic -Wno-padded -fno-exceptions -fno-rtti )

target_compile_options( MobileRT PRIVATE -Wno-unused-variable -Wno-unused-function -Wno-float-equal -funsafe-math-optimizations -ffinite-math-only -fno-signed-zeros -fno-trapping-math -fno-math-errno -ffunction-sections -fdata-sections -fno-stack-protector -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident )

target_compile_options( MobileRT PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full>)
if( NOT ANDROID_ABI MATCHES "^mips(64)?$" )
    set(GOLD "-fuse-ld=gold" )
endif()

target_link_libraries( MobileRT
PUBLIC log atomic PRIVATE --verbose
general -Wl,-O3
general -Wl,-discard-all
general -Wl,-no-undefined
general -Wl,-gc-sections
general -Wl,-z,norelro
general -Wl,-hash-style=sysv
general -gsplit-dwarf
general -flto=full ${GOLD}
optimized -Wl,-build-id=none
optimized -Wl,-s
)
