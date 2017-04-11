cmake_minimum_required(VERSION 3.6.0)

set(ANDROID_ARM_NEON TRUE)
set(LOCAL_ARM_NEON TRUE)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
add_compile_options(-std=c++11)
add_compile_options(-Weverything -Werror -Wfatal-errors)
add_compile_options(-Wno-c++98-compat-pedantic -Wno-padded -Wno-unused-variable)
add_compile_options(-Wno-unused-private-field)
add_compile_options(-Ofast)
#add_compile_options(-O0)
add_compile_options(-g3 -funwind-tables -fexceptions -frtti)
add_compile_options(-I./src/main/cpp)
include_directories(./src/main/cpp)

file(GLOB_RECURSE MOBILE_RC_SOURCES "${CMAKE_SOURCE_DIR}/src/main/cpp/**.cpp")
add_library(Components SHARED ${MOBILE_RC_SOURCES})

string(TOLOWER ${CMAKE_BUILD_TYPE} FOLDER_TYPE)

set(LIB_PATH ${CMAKE_SOURCE_DIR}/../MobileRT/build/intermediates/cmake/${FOLDER_TYPE}/obj)
add_library(MobileRT STATIC IMPORTED)
set_property(TARGET MobileRT PROPERTY IMPORTED_LOCATION ${LIB_PATH}/${ANDROID_ABI}/libMobileRT.so)

target_link_libraries(Components MobileRT log atomic)
