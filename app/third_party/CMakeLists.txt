cmake_minimum_required( VERSION 3.6.0 )

add_subdirectory( glm )
add_subdirectory( googletest )
add_subdirectory( tinyobjloader )

if( NOT DEFINED ANDROID_NDK )
  add_subdirectory( GSL )
endif()
