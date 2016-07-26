project(dbo-tests)

find_package(Boost 1.53 COMPONENTS date_time thread program_options REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR};${ROOT_PATH})
FIND_PACKAGE(Postgresql REQUIRED)
FIND_PACKAGE(GoogleTest REQUIRED)
add_definitions(-DMULTI_THREADED)

include_directories(
    ${GoogleTest_INCLUDE_DIR}
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
    ${GoogleTest_LIBRARY_DIR}
    ${Boost_LIBRARY_DIRS} 
    )

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} 
	gtest
	gtest_main
	pthread
 
    dbo-lib

    ${Boost_LIBRARIES} 
 
    ${Postgres_LIBRARIES} 
    
    gdal
    )

enable_testing()
add_test(tests Build/bin/${PROJECT_NAME})
