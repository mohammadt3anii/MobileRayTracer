cmake_minimum_required( VERSION 3.6.0 )

include( "${CMAKE_CURRENT_LIST_DIR}/../MobileRT/CMakeLists.txt" )
include( "${CMAKE_CURRENT_LIST_DIR}/../../third-party/tinyobjloader/CMakeLists_Android.txt" )

set( CMAKE_VERBOSE_MAKEFILE on )
set( LIBCXX_FORCE_REBUILD true )
set( STLPORT_FORCE_REBUILD true )
set( LOCAL_ARM_NEON true )
set( ANDROID_ARM_NEON true )
set( ANDROID_TOOLCHAIN clang )
set( ANDROID_STL c++_shared )
set( ANDROID_ARM_MODE thumb )
set( ANDROID_DISABLE_NO_EXECUTE true )
set( ANDROID_DISABLE_FORMAT_STRING_CHECKS true )
set( ANDROID_PIE on )

file( GLOB_RECURSE MOBILE_RC_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/**.cpp" )
add_library( Components SHARED ${MOBILE_RC_SOURCES} )
target_include_directories( Components INTERFACE "${CMAKE_CURRENT_LIST_DIR}/src/main/cpp" )
target_include_directories( Components PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../third-party" )

target_compile_options( Components PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )
target_compile_options( Components PRIVATE $<$<CONFIG:RELEASE>:-Ofast -DNDEBUG> )

target_compile_options( Components PRIVATE -std=c++14 -Weverything -Wall -Wfatal-errors --verbose -Werror -ferror-limit=1 -gsplit-dwarf )
target_compile_options( Components PRIVATE -Wno-c++98-compat-pedantic -Wno-padded -fno-exceptions -fno-rtti )

target_compile_options( Components PRIVATE -Wno-unused-variable -Wno-unused-function -Wno-unused-private-field -Wno-over-aligned )
target_compile_options( Components PRIVATE -Wno-float-equal -funsafe-math-optimizations -ffinite-math-only -fno-signed-zeros -fno-trapping-math -fno-math-errno -ffunction-sections -fdata-sections -fno-stack-protector -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident -ftemplate-depth=10000000 )

target_compile_options( Components PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64) ?$" >>:-flto=full> )
if(NOT ANDROID_ABI MATCHES "^mips(64)?$" )
    set(GOLD "-fuse-ld=gold" )
endif()

target_link_libraries( Components
PUBLIC MobileRT tinyobjloader PRIVATE --verbose
general -Wl,-O3
general -Wl,-discard-all
general -Wl,-no-undefined
general -Wl,-gc-sections
general -Wl,-z,norelro
general -Wl,-hash-style=sysv
general -Wl,-flto=full ${GOLD}
general -Wl,-fno-rtti
general -Wl,-fno-exceptions
optimized -Wl,-s
optimized -Wl,-build-id=none
)
