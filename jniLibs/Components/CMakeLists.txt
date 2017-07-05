cmake_minimum_required(VERSION 3.6.0)

include(${PROJECT_PATH}/jniLibs/MobileRT/CMakeLists.txt)

set(CMAKE_VERBOSE_MAKEFILE on)
set(LOCAL_ARM_NEON TRUE)
set(ANDROID_ARM_NEON TRUE)
set(ANDROID_TOOLCHAIN clang)
set(ANDROID_STL c++_static)

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_compile_options(-std=c++14 -Weverything -Werror -Wfatal-errors -v)
add_compile_options(-Wno-c++98-compat-pedantic -Wno-padded)
add_compile_options(-fno-exceptions -fno-rtti -Wno-reserved-id-macro)

file(GLOB_RECURSE MOBILE_RC_SOURCES "${PROJECT_PATH}/jniLibs/Components/src/main/cpp/**.cpp")
add_library(Components SHARED ${MOBILE_RC_SOURCES})
target_include_directories(Components INTERFACE ${PROJECT_PATH}/jniLibs/Components/src/main/cpp)

target_link_libraries(Components MobileRT log atomic)
