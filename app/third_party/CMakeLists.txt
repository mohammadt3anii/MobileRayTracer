cmake_minimum_required( VERSION 3.6.0 )

execute_process( COMMAND git clone https://github.com/g-truc/glm
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

execute_process( COMMAND git clone https://github.com/google/googletest
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

execute_process( COMMAND git clone https://github.com/syoyo/tinyobjloader
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

execute_process( COMMAND git clone https://github.com/Microsoft/GSL
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-double-promotion -Wno-undef" CACHE STRING "CUSTOM" FORCE )
add_subdirectory( glm )
set( CMAKE_CXX_FLAGS "-fPIC" CACHE STRING "CUSTOM" FORCE )
add_subdirectory( googletest )
add_subdirectory( tinyobjloader )

if( NOT DEFINED ANDROID_ABI )
  add_subdirectory( GSL )
endif()
