cmake_minimum_required(VERSION 3.6.0)

set(ANDROID_ARM_NEON TRUE)
set(LOCAL_ARM_NEON TRUE)

add_compile_options(-std=c++11)
add_compile_options(-Weverything -Werror -Wfatal-errors -Wno-c++98-compat-pedantic)
add_compile_options(-Ofast)

file(GLOB_RECURSE MOBILE_APP_SOURCES "${CMAKE_SOURCE_DIR}/src/main/cpp/**.cpp")
add_library(DrawView SHARED ${MOBILE_APP_SOURCES})

string(TOLOWER ${CMAKE_BUILD_TYPE} FOLDER_TYPE)

set(LIB_PATH ${CMAKE_SOURCE_DIR}/../jniLibs/MobileRT/build/intermediates/cmake/${FOLDER_TYPE}/obj)
add_library(MobileRT STATIC IMPORTED)
set_property(TARGET MobileRT PROPERTY IMPORTED_LOCATION ${LIB_PATH}/${ANDROID_ABI}/libMobileRT.so)

set(COMP_PATH ${CMAKE_SOURCE_DIR}/../jniLibs/Components/build/intermediates/cmake/${FOLDER_TYPE}/obj)
add_library(Components STATIC IMPORTED)
set_property(TARGET Components PROPERTY IMPORTED_LOCATION ${COMP_PATH}/${ANDROID_ABI}/libComponents.so)

target_link_libraries(DrawView jnigraphics MobileRT log)