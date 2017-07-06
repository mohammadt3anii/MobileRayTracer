cmake_minimum_required(VERSION 3.6.0)

include(${PROJECT_PATH}/jniLibs/Components/CMakeLists.txt)

set(CMAKE_VERBOSE_MAKEFILE on)
set(LOCAL_ARM_NEON TRUE)
set(ANDROID_ARM_NEON TRUE)
set(ANDROID_TOOLCHAIN clang)
set(ANDROID_STL gnustl_shared)
set(STLPORT_FORCE_REBUILD TRUE)
set(LIBCXX_FORCE_REBUILD TRUE)
set(ANDROID_ARM_MODE THUMB)
set(ANDROID_DISABLE_NO_EXECUTE TRUE)
set(ANDROID_DISABLE_FORMAT_STRING_CHECKS TRUE)
set(ANDROID_PIE ON)

file(GLOB_RECURSE MOBILE_APP_SOURCES "${PROJECT_PATH}/app/src/main/cpp/**.cpp")
add_library(DrawView SHARED ${MOBILE_APP_SOURCES})

target_compile_options(DrawView PRIVATE $<$<CONFIG:DEBUG>:-g3 -O0>)
target_compile_options(DrawView PRIVATE $<$<CONFIG:RELEASE>:-Ofast>)

target_compile_options(DrawView PRIVATE -std=c++14 -Weverything -Werror -Wfatal-errors --verbose)
target_compile_options(DrawView PRIVATE -Wno-c++98-compat-pedantic -Wno-padded -Wno-over-aligned)
target_compile_options(DrawView PRIVATE -fno-exceptions -fno-rtti)

target_compile_options(DrawView PRIVATE $<$<CONFIG:RELEASE>:-fno-stack-protector -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-math-errno -fno-ident>)

target_link_libraries(DrawView
PRIVATE jnigraphics Components --verbose
optimized -Wl,--gc-sections
optimized -Wl,-z,norelro
optimized -Wl,--hash-style=sysv
optimized -Wl,--build-id=none
optimized -s)
