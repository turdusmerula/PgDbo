# tcmalloc flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -fno-inline-functions -fno-inline")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -fno-inline-functions -fno-inline")

# add tcmalloc as last lib for each linking target
#set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -l:/opt/gperftools/.libs/libtcmalloc.so")
#set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -Wl,-rpath=/opt/gperftools/.libs -L/opt/gperftools/.libs -ltcmalloc")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -L/opt/gperftools/.libs -ltcmalloc")

# add search path for include
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/gperftools/src")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/opt/gperftools/src")

# add compile flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTCMALLOC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTCMALLOC")
