cmake_minimum_required(VERSION 3.6.0)

file(TO_CMAKE_PATH ${PROJECT_PATH} PROJECT_PATH)

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

set(MOBILE_RT_PATH ${PROJECT_PATH}/jniLibs/MobileRT/src/main/cpp)
file(GLOB_RECURSE MOBILE_RT_SOURCES "${MOBILE_RT_PATH}/**.cpp")
add_library(MobileRT SHARED ${MOBILE_RT_SOURCES})
target_include_directories(MobileRT INTERFACE ${MOBILE_RT_PATH})

target_compile_options(MobileRT PRIVATE $<$<CONFIG:DEBUG>:-g3 -O0>)
target_compile_options(MobileRT PRIVATE $<$<CONFIG:RELEASE>:-Ofast>)

target_compile_options(MobileRT PRIVATE -std=c++14 -Weverything -Werror -Wfatal-errors --verbose)
target_compile_options(MobileRT PRIVATE -Wno-c++98-compat-pedantic -Wno-padded)
target_compile_options(MobileRT PRIVATE -fno-exceptions -fno-rtti)

target_compile_options(MobileRT PRIVATE $<$<CONFIG:RELEASE>:-fno-stack-protector -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident>)
target_compile_options(MobileRT PRIVATE $<$<CONFIG:RELEASE>:-fno-math-errno -ffunction-sections -fdata-sections>)

target_link_libraries(MobileRT
PUBLIC log atomic PRIVATE --verbose
optimized -Wl,-O3
optimized -Wl,-discard-all
optimized -Wl,-no-undefined
optimized -Wl,-gc-sections
optimized -Wl,-ffunction-sections
optimized -Wl,-fdata-sections
optimized -Wl,-z,norelro
optimized -Wl,-hash-style=sysv
optimized -Wl,-build-id=none
optimized -Wl,-s
optimized -Wl,-flto)