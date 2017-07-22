cmake_minimum_required(VERSION 3.6.0)

file(TO_CMAKE_PATH ${PROJECT_PATH} PROJECT_PATH)
include(${PROJECT_PATH}/jniLibs/Components/CMakeLists.txt)

set(CMAKE_VERBOSE_MAKEFILE on)
set(LIBCXX_FORCE_REBUILD FALSE)
set(STLPORT_FORCE_REBUILD FALSE)
set(LOCAL_ARM_NEON TRUE)
set(ANDROID_ARM_NEON TRUE)
set(ANDROID_TOOLCHAIN clang)
set(ANDROID_STL c++_shared)
set(ANDROID_ARM_MODE THUMB)
set(ANDROID_DISABLE_NO_EXECUTE TRUE)
set(ANDROID_DISABLE_FORMAT_STRING_CHECKS TRUE)
set(ANDROID_PIE ON)

set(ANDROID_COMPILER_FLAGS_DEBUG "")
set(ANDROID_COMPILER_FLAGS_RELEASE "")
set(ANDROID_LINKER_FLAGS "")
set(ANDROID_LINKER_FLAGS_EXE "")
set(CMAKE_ASM_FLAGS_DEBUG "")
set(CMAKE_ASM_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "")
set(CMAKE_CXX_FLAGS_MINSIZEREL "")
set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_DEBUG_INIT "")
set(CMAKE_C_FLAGS_MINSIZEREL "")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_RELEASE_INIT "")

set(MOBILE_APP_PATH ${PROJECT_PATH}/app/src/main/cpp)
file(GLOB_RECURSE MOBILE_APP_SOURCES "${MOBILE_APP_PATH}/**.cpp")
add_library(DrawView SHARED ${MOBILE_APP_SOURCES})

target_compile_options(DrawView PRIVATE $<$<CONFIG:DEBUG>:-O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls>)
target_compile_options(DrawView PRIVATE $<$<CONFIG:RELEASE>:-Ofast>)

target_compile_options(DrawView PRIVATE -std=c++14 -Weverything -Wfatal-errors --verbose -Werror)
target_compile_options(DrawView PRIVATE -Wno-c++98-compat-pedantic -Wno-padded -Wno-over-aligned)
target_compile_options(DrawView PRIVATE -fno-exceptions -fno-rtti -Wno-unused-variable -Wno-unused-function -Wno-unused-command-line-argument -Wno-weak-vtables)

target_compile_options(DrawView PRIVATE $<$<CONFIG:RELEASE>:-fno-stack-protector -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident>)
target_compile_options(DrawView PRIVATE $<$<CONFIG:RELEASE>:-fno-math-errno -ffunction-sections -fdata-sections>)

target_link_libraries(DrawView
PRIVATE jnigraphics Components --verbose
optimized -Wl,-O3
optimized -Wl,-discard-all
optimized -Wl,-no-undefined
optimized -Wl,-gc-sections
optimized -Wl,-z,norelro
optimized -Wl,-hash-style=sysv
optimized -Wl,-build-id=none
optimized -Wl,-s
debug -Wl,-O0
debug -Wl,-g
debug -Wl,-fno-omit-frame-pointer
debug -Wl,-fno-optimize-sibling-calls
)
