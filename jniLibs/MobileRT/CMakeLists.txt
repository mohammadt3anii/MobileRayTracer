cmake_minimum_required(VERSION 3.6.0)

set(ANDROID_ARM_NEON TRUE)
set(LOCAL_ARM_NEON TRUE)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
add_compile_options(-std=c++11)
add_compile_options(-Weverything -Werror -Wfatal-errors)
add_compile_options(-Wno-c++98-compat-pedantic -Wno-padded)
add_compile_options(-Ofast)
#add_compile_options(-O0)
add_compile_options(-g3 -funwind-tables -fexceptions -frtti)
add_compile_options(-I./src/main/cpp)
include_directories(./src/main/cpp)

file(GLOB_RECURSE MOBILE_RT_SOURCES "${CMAKE_SOURCE_DIR}/src/main/cpp/**.cpp")
add_library(MobileRT SHARED ${MOBILE_RT_SOURCES})

target_link_libraries(MobileRT log atomic)
