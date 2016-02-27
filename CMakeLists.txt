cmake_minimum_required(VERSION 2.8)
project(Bdo)

#Set build path relative to each project
set(CMAKE_BUILD_DIRECTORY Build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Build/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Build/bin)
set(PACKAGE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Build/pkg)
set(RESOURCES_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Build/resources)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(cmake/version.cmake)

# read version
get_software_version()
message("Build software version ${software_version}")

# Cots path
set(COTS_PATH /opt)

# Root path
set(ROOT_PATH ${CMAKE_SOURCE_DIR})

# boost path in case of non system boost usage
if(DEFINED BOOST_ROOT)
    set(Boost_NO_SYSTEM_PATHS ON)
endif()


# set default compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-deprecated-declarations -Wno-unused-command-line-argument")

# optimisation for build speed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")

# additional configuration for debug/release mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	#Compile with debug flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	#Compile with optimisations on, -O3 mode is not recommended
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# select custom linker
if(DEFINED TOOLCHAIN_LINKER)
    include(cmake/config/linker-${TOOLCHAIN_LINKER}.cmake)
endif()

# select profiler
if(DEFINED TOOLCHAIN_PROFILER)
    include(cmake/config/profiler-${TOOLCHAIN_PROFILER}.cmake)
endif()

SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CTEST_LAUNCH_COMPILE}")

message("Build type ${CMAKE_BUILD_TYPE}")
message("Compile options ${CMAKE_CXX_FLAGS}")


#Build
add_subdirectory(dbo)
add_subdirectory(wtdbo)
add_subdirectory(dbo-tests)
add_subdirectory(wtdbo-tests)

# test
include( CTest )
enable_testing()
