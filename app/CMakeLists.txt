cmake_minimum_required(VERSION 3.6.0)

include(${PROJECT_PATH}/jniLibs/Components/CMakeLists.txt)

set(CMAKE_VERBOSE_MAKEFILE on)
set(LOCAL_ARM_NEON TRUE)
set(ANDROID_ARM_NEON TRUE)
set(ANDROID_TOOLCHAIN clang)
set(ANDROID_STL c++_static)

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_compile_options(-std=c++14 -Weverything -Werror -Wfatal-errors --verbose)
add_compile_options(-Wno-c++98-compat-pedantic -Wno-padded -Wno-over-aligned)
add_compile_options(-fno-exceptions -fno-rtti -Wno-reserved-id-macro)

file(GLOB_RECURSE MOBILE_APP_SOURCES "${PROJECT_PATH}/app/src/main/cpp/**.cpp")
add_library(DrawView SHARED ${MOBILE_APP_SOURCES})

target_link_libraries(DrawView jnigraphics MobileRT Components log --verbose)
