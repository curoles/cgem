add_executable (test_stack_array
    test_stack_array.cpp
)

target_link_libraries(test_stack_array
    GTest::GTest GTest::Main
)

gtest_discover_tests(test_stack_array)
