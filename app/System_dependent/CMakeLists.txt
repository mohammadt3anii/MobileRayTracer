cmake_minimum_required( VERSION 3.6.0 )

if( DEFINED ANDROID_ABI )
	file( GLOB_RECURSE MOBILE_DEPENDENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Android_JNI/**.cpp" )
else()
	file( GLOB_RECURSE MOBILE_DEPENDENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Linux/**.cpp" )
endif()

set( MOBILE_DEPENDENT_SOURCES "${MOBILE_DEPENDENT_SOURCES}" CACHE STRING "CUSTOM" FORCE )
