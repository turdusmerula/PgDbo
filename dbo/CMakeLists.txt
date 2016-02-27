project(dbo-lib)

find_package(Boost 1.53 COMPONENTS filesystem regex system date_time thread REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR})
FIND_PACKAGE(Postgresql REQUIRED)
add_definitions(-DMULTI_THREADED)

include_directories(
    ${Postgres_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
	..
    )

LINK_DIRECTORIES(
    ${Boost_LIBRARY_DIRS}
)

FILE(GLOB sources 
    *.cpp
    action/*.cpp
    mapping/*.cpp
    stmt/*.cpp
    traits/*.cpp
    )
    
FILE(GLOB headers 
    src/*.h
    )

add_library(${PROJECT_NAME} SHARED ${sources})

target_link_libraries(${PROJECT_NAME} 
    pthread

    ${Boost_LIBRARIES} 

    ${Postgres_LIBRARIES}

    gdal

    dl
    )
