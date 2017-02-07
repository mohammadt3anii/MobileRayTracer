cmake_minimum_required(VERSION 3.4.1)

set(ANDROID_ARM_NEON TRUE)
set(ANDROID_TOOLCHAIN clang)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fexceptions -Wpedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winline -Wfatal-errors -Ofast -ftree-vectorize -ffast-math")

file(GLOB_RECURSE MOBILE_RT_SOURCES "src/main/cpp/*/*.cpp")

add_library(MobileRT_objects OBJECT ${MOBILE_RT_SOURCES})
set_property(TARGET ${MobileRT_objects} PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(MobileRT SHARED $<TARGET_OBJECTS:MobileRT_objects>)

set_target_properties(MobileRT PROPERTIES OUTPUT_NAME MobileRT CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(MobileRT)