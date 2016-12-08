project(dbo-lib)

find_package(Boost 1.53 COMPONENTS filesystem regex system date_time thread REQUIRED)

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
    ogr/*.cpp
    stmt/*.cpp
    traits/*.cpp
    )

FILE(GLOB headers 
    src/*.h
    )

add_library(dbo SHARED ${sources})

target_link_libraries(dbo
    pthread

    ${Boost_LIBRARIES} 

    ${Postgres_LIBRARIES}
    
    gdal

    dl
    )

install(DIRECTORY . DESTINATION include/dbo FILES_MATCHING PATTERN "*.hpp" PATTERN "*.cxx" PATTERN "*.h")
install(TARGETS dbo EXPORT DESTINATION lib)
