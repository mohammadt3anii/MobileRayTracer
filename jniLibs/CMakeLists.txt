cmake_minimum_required( VERSION 3.6.0 )

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DNO_ANDROID -fPIC" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem /usr/include/c++/v1" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I./MobileRT/src/main/cpp" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I./Components/src/main/cpp" )

set( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -fno-optimize-sibling-calls -fno-omit-frame-pointer" )

set( CMAKE_SHARED_LINKER_FLAGS "-fPIC -lc++ -pthread -ltinyobjloader" )

link_directories( ${CMAKE_BINARY_DIR}/lib )

find_package( GTK2 REQUIRED )

include_directories(
    ${GTK2_INCLUDE_DIR}
)

link_libraries(
  ${GTK2_LIBRARIES}
)

add_subdirectory( third-party )
add_subdirectory( MobileRT )
add_subdirectory( Components )