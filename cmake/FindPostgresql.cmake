# This file defines:
# - Postgres_LIBRARIES
# - Postgres_INCLUDE_DIRS
# - Postgres_FOUND
# Taking into account:
# - POSTGRES_PREFIX

SET(Postgres_LIBRARIES pq)

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

