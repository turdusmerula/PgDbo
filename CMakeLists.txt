cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0057 NEW)

project(PgDbo)

#Set build path relative to each project
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

include(cmake/version.cmake)

# read version
get_software_version()
message("Build software version ${software_version}")


# Root path
set(ROOT_PATH ${CMAKE_SOURCE_DIR})

# boost path in case of non system boost usage
if(DEFINED BOOST_ROOT)
    set(Boost_NO_SYSTEM_PATHS ON)
endif()


# set default compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wno-deprecated-declarations -Wno-unused-command-line-argument")

# optimisation for build speed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")

message("Build type ${CMAKE_BUILD_TYPE}")
message("Compile options ${CMAKE_CXX_FLAGS}")

#Build
add_subdirectory(dbo)
add_subdirectory(dbo-tests)

# test
include( CTest )
enable_testing()
