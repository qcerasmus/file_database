add_executable(database_tests database_tests.cpp)

target_link_libraries(
	database_tests
	PUBLIC
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(database_tests)
