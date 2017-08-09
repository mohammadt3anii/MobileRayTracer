cmake_minimum_required( VERSION 3.6.0 )

include( "${CMAKE_CURRENT_LIST_DIR}/../jniLibs/Components/CMakeLists.txt" )

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

file( GLOB_RECURSE MOBILE_APP_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/**.cpp" )
add_library( DrawView SHARED ${MOBILE_APP_SOURCES} )

target_compile_options( DrawView PRIVATE $<$<CONFIG:DEBUG>:-O0 -g -fno-optimize-sibling-calls> )
target_compile_options( DrawView PRIVATE $<$<CONFIG:RELEASE>:-Ofast -DNDEBUG> )

target_compile_options( DrawView PRIVATE -std=c++14 -Weverything -Wall -Wfatal-errors --verbose -Werror -ferror-limit=1 -gsplit-dwarf )
target_compile_options( DrawView PRIVATE -Wno-c++98-compat-pedantic -Wno-padded -Wno-over-aligned -fno-exceptions -fno-rtti )

target_compile_options( DrawView PRIVATE -Wno-unused-variable -Wno-unused-function -Wno-float-equal -funsafe-math-optimizations -ffinite-math-only -fno-signed-zeros -fno-trapping-math -fno-math-errno -ffunction-sections -fdata-sections -fno-stack-protector -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident -static )

target_compile_options( DrawView PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64) ?$" >>:-flto=full> )
if( NOT ANDROID_ABI MATCHES "^mips(64)?$" )
    set( GOLD "-fuse-ld=gold" )
endif()

target_link_libraries( DrawView
PRIVATE jnigraphics Components --verbose
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
