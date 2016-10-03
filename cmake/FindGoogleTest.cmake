SET( GoogleTest_FIND_COMPONENTS Release Debug )

set(GoogleTest_COTS_DIR /opt/googletest)

message("GoogleTest_COTS_DIR ${GoogleTest_COTS_DIR}")
SET( GoogleTest_LIBRARY_DIR		"${GoogleTest_COTS_DIR}/googlemock/gtest" )
SET( GoogleTest_LIBRARIES 		"-lgtest -lgtest_main" )

SET( GoogleTest_INCLUDE_DIR 		${GoogleTest_COTS_DIR}/googletest/include)
