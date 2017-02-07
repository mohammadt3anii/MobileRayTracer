cmake_minimum_required(VERSION 3.4.1)

set(ANDROID_ARM_NEON TRUE)
set(ANDROID_TOOLCHAIN clang)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fexceptions -Wpedantic -Wall -Wextra -ljnigraphics")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winline -Wfatal-errors -Ofast -ftree-vectorize -ffast-math")

file(GLOB_RECURSE MOBILE_APP_SOURCES "src/main/cpp/DrawView.cpp")

add_library(DrawView_objects OBJECT ${MOBILE_APP_SOURCES})
set_property(TARGET ${DrawView_objects} PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(DrawView SHARED $<TARGET_OBJECTS:DrawView_objects>)

add_library(MobileRT STATIC IMPORTED)
string(TOLOWER ${CMAKE_BUILD_TYPE} FOLDER_TYPE)
set(LIB_PATH ${CMAKE_SOURCE_DIR}/../jniLibs/MobileRT/build/intermediates/cmake/${FOLDER_TYPE}/obj)
set_target_properties(MobileRT PROPERTIES IMPORTED_LOCATION ${LIB_PATH}/${ANDROID_ABI}/libMobileRT.so)

target_link_libraries(DrawView MobileRT log)