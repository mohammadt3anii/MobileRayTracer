cmake_minimum_required(VERSION 3.6.0)

include(${PROJECT_PATH}/jniLibs/MobileRT/CMakeLists.txt)

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

file(GLOB_RECURSE MOBILE_RC_SOURCES "${PROJECT_PATH}/jniLibs/Components/src/main/cpp/**.cpp")
add_library(Components SHARED ${MOBILE_RC_SOURCES})
target_include_directories(Components INTERFACE ${PROJECT_PATH}/jniLibs/Components/src/main/cpp)

target_compile_options(Components PRIVATE $<$<CONFIG:DEBUG>:-g3 -O0>)
target_compile_options(Components PRIVATE $<$<CONFIG:RELEASE>:-Ofast>)

target_compile_options(Components PRIVATE -std=c++14 -Weverything -Werror -Wfatal-errors --verbose)
target_compile_options(Components PRIVATE -Wno-c++98-compat-pedantic -Wno-padded)
target_compile_options(Components PRIVATE -fno-exceptions -fno-rtti)

target_compile_options(Components PRIVATE $<$<CONFIG:RELEASE>:-fno-stack-protector -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-math-errno -fno-ident>)

target_link_libraries(Components
PUBLIC MobileRT PRIVATE --verbose
optimized -Wl,--gc-sections
optimized -Wl,-z,norelro
optimized -Wl,--hash-style=sysv
optimized -Wl,--build-id=none
optimized -s)
