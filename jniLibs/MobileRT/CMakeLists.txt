cmake_minimum_required(VERSION 3.6.0)

set(ANDROID_ARM_NEON TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra -Winline -Wfatal-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ftree-vectorize -ffast-math")

file(GLOB_RECURSE MOBILE_RT_SOURCES "${CMAKE_SOURCE_DIR}/src/main/cpp/**.cpp")

add_library(MobileRT SHARED ${MOBILE_RT_SOURCES})

target_link_libraries(MobileRT)