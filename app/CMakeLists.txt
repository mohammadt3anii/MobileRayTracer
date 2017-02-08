cmake_minimum_required(VERSION 3.6.0)

set(ANDROID_ARM_NEON TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ljnigraphics")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra -Winline -Wfatal-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ftree-vectorize -ffast-math")

file(GLOB_RECURSE MOBILE_APP_SOURCES "${CMAKE_SOURCE_DIR}/src/main/cpp/DrawView.cpp")

add_library(DrawView SHARED ${MOBILE_APP_SOURCES})

string(TOLOWER ${CMAKE_BUILD_TYPE} FOLDER_TYPE)
set(LIB_PATH ${CMAKE_SOURCE_DIR}/../jniLibs/MobileRT/build/intermediates/cmake/${FOLDER_TYPE}/obj)

add_library(MobileRT STATIC IMPORTED)
set_property(TARGET MobileRT PROPERTY IMPORTED_LOCATION ${LIB_PATH}/${ANDROID_ABI}/libMobileRT.so)

target_link_libraries(DrawView MobileRT log)