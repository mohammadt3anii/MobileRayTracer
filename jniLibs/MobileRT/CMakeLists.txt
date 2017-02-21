cmake_minimum_required(VERSION 3.6.0)

set(ANDROID_ARM_NEON TRUE)
set(LOCAL_ARM_NEON TRUE)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
add_compile_options(-std=c++11)
add_compile_options(-Weverything -Werror -Wfatal-errors -Wno-c++98-compat-pedantic)
add_compile_options(-Ofast)

file(GLOB_RECURSE MOBILE_RT_SOURCES "${CMAKE_SOURCE_DIR}/src/main/cpp/**.cpp")
add_library(MobileRT SHARED ${MOBILE_RT_SOURCES})

target_link_libraries(MobileRT log)
