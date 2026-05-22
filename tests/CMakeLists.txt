find_package(GTest REQUIRED)

add_executable(bulk_average_test bulk_average_test.cpp)
target_link_libraries(bulk_average_test PRIVATE bulk_average GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(bulk_average_test)
