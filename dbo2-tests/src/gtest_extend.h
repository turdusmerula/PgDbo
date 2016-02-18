#ifndef GTEST_INCLUDE_GTEST_EXTEND_GTEST_H_
#define GTEST_INCLUDE_GTEST_EXTEND_GTEST_H_

#include <iostream>

#define NONE "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"

#define ASSERT_THROW_V(statement, expected_exception) \
  GTEST_TEST_THROW_( \
	try{ \
		statement ; \
	} catch(expected_exception& e) { \
		std::cout << GREEN << "[ THROWED  ] " << NONE << e.what() << std::endl ; \
		throw ;\
	}, expected_exception, GTEST_FATAL_FAILURE_)

#define ASSERT_NO_THROW_V(statement) \
  GTEST_TEST_NO_THROW_( \
	try{ \
		statement ; \
	} catch(std::exception& e) { \
		std::cout << RED << "[ THROWED  ] " << NONE << e.what() << std::endl ; \
		throw ;\
	}, GTEST_NONFATAL_FAILURE_)

#endif
