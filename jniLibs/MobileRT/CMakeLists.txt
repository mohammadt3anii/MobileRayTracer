cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_VERBOSE_MAKEFILE on)
set(ANDROID_ARM_NEON TRUE)
set(LOCAL_ARM_NEON TRUE)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og -fno-omit-frame-pointer -fno-limit-debug-info -ftrapv")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_compile_options(-std=c++11 -Weverything -Werror -Wfatal-errors)
add_compile_options(-Wno-c++98-compat-pedantic -Wno-padded)
add_compile_options(-Wno-unused-private-field)

include_directories(./src/main/cpp)

file(GLOB_RECURSE MOBILE_RT_SOURCES "${CMAKE_SOURCE_DIR}/src/main/cpp/**.cpp")
add_library(MobileRT SHARED ${MOBILE_RT_SOURCES})

target_link_libraries(MobileRT log atomic)
