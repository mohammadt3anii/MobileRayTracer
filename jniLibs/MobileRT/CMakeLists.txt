cmake_minimum_required(VERSION 3.6.0)

file(TO_CMAKE_PATH ${PROJECT_PATH} PROJECT_PATH)

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
#message(FATAL_ERROR "ACABOU")

STRING(REGEX REPLACE "-Wa,--noexecstack" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

set(CMAKE_VERBOSE_MAKEFILE on)
set(LIBCXX_FORCE_REBUILD FALSE)
set(STLPORT_FORCE_REBUILD FALSE)
set(LOCAL_ARM_NEON TRUE)
set(ANDROID_ARM_NEON TRUE)
set(ANDROID_TOOLCHAIN clang)
set(ANDROID_STL c++_shared)
set(ANDROID_ARM_MODE THUMB)
set(ANDROID_DISABLE_NO_EXECUTE TRUE)
set(ANDROID_DISABLE_FORMAT_STRING_CHECKS TRUE)
set(ANDROID_PIE ON)

set(MOBILE_RT_PATH ${PROJECT_PATH}/jniLibs/MobileRT/src/main/cpp)
file(GLOB_RECURSE MOBILE_RT_SOURCES "${MOBILE_RT_PATH}/**.cpp")
add_library(MobileRT SHARED ${MOBILE_RT_SOURCES})
target_include_directories(MobileRT INTERFACE ${MOBILE_RT_PATH})

target_compile_options(MobileRT PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls>)
target_compile_options(MobileRT PRIVATE $<$<CONFIG:RELEASE>:-Ofast>)

target_compile_options(MobileRT PRIVATE -std=c++14 -Weverything -Wfatal-errors --verbose -Werror)
target_compile_options(MobileRT PRIVATE -Wno-c++98-compat-pedantic -Wno-padded)
target_compile_options(MobileRT PRIVATE -fno-exceptions -fno-rtti -Wno-unused-variable -Wno-unused-function -Wno-float-equal)

target_compile_options(MobileRT PRIVATE $<$<CONFIG:RELEASE>:-fno-stack-protector -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident>)
target_compile_options(MobileRT PRIVATE $<$<CONFIG:RELEASE>:-fno-math-errno -ffunction-sections -fdata-sections>)

target_link_libraries(MobileRT
PUBLIC log atomic PRIVATE --verbose
general -Wl,-O3
general -Wl,-discard-all
general -Wl,-no-undefined
general -Wl,-gc-sections
general -Wl,-z,norelro
general -Wl,-hash-style=sysv
optimized -Wl,-build-id=none
optimized -Wl,-s
)
