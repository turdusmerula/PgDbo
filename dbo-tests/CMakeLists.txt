project(dbo-tests)

find_package(Boost 1.53 COMPONENTS date_time thread program_options REQUIRED)

FIND_PACKAGE(Postgresql REQUIRED)
FIND_PACKAGE(GTest REQUIRED)
add_definitions(-DMULTI_THREADED)

include_directories(
    ${GTEST_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
	src
	${CMAKE_SOURCE_DIR}
)

FILE(GLOB sources 
    src/main.cpp
    )

FILE(GLOB headers 
    src/*.hpp
    )

LINK_DIRECTORIES(
    ${Boost_LIBRARY_DIRS} 
    )

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} 
    ${GTEST_LIBRARIES}
    
	pthread
 
    dbo

    ${Boost_LIBRARIES} 
 
    ${Postgres_LIBRARIES} 
    
    gdal
    )

enable_testing()
file(GLOB test_list
    src/*.hpp
)
GTEST_ADD_TESTS(${PROJECT_NAME} "" ${test_list})
