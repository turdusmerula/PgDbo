#Ftrace debug flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -finstrument-functions -fno-inline-functions -fno-inline")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -finstrument-functions -fno-inline-functions -fno-inline")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")
