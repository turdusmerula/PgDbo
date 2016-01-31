# This file defines:
# - Postgres_LIBRARIES
# - Postgres_INCLUDE_DIRS
# - Postgres_FOUND
# Taking into account:
# - POSTGRES_PREFIX

SET(Postgres_LIBRARIES pq libpq)
FOREACH(l ${Postgres_LIBRARIES})
  LIST(APPEND POSTGRES_DEBUG_LIBRARY "${l}d")
ENDFOREACH()

FIND_LIBRARY(POSTGRES_LIB
  NAMES
    ${Postgres_LIBRARIES}
  PATHS
    ${POSTGRES_PREFIX}/lib
    ${POSTGRES_PREFIX}
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    /usr/lib64
    /usr/local/lib
    /opt/local/lib
)

FIND_LIBRARY(POSTGRES_DEBUG_LIB
  NAMES
    ${POSTGRES_DEBUG_LIBRARY}
  PATHS
    ${POSTGRES_PREFIX}/lib
    ${POSTGRES_PREFIX}
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    /usr/lib64
    /usr/local/lib
    /opt/local/lib
)

IF (POSTGRES_DEBUG_LIB)
  SET(Postgres_LIBRARIES optimized ${POSTGRES_LIB} debug ${POSTGRES_DEBUG_LIB})
ELSE (POSTGRES_DEBUG_LIB)
  SET(Postgres_LIBRARIES ${POSTGRES_LIB})
ENDIF (POSTGRES_DEBUG_LIB)

FIND_PATH(POSTGRES_INCLUDE libpq-fe.h
    ${POSTGRES_PREFIX}/include
    ${POSTGRES_PREFIX}/postgresql/include
    ${POSTGRES_PREFIX}/include/postgresql
    /usr/include
    /usr/include/pgsql
    /usr/include/postgresql
    /usr/local/include
    /usr/local/include/postgresql
    /opt/local/include	
    /opt/local/include/postgresql
)

SET(POSTGRES_FOUND FALSE)

IF(Postgres_LIBRARIES AND POSTGRES_INCLUDE)
  SET(Postgres_FOUND TRUE)
  SET(Postgres_INCLUDE_DIRS ${POSTGRES_INCLUDE})
ENDIF(Postgres_LIBRARIES AND POSTGRES_INCLUDE)

