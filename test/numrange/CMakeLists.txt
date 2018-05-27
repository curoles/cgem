add_executable (test_numrange
    test_numrange.cpp
)

target_link_libraries(test_numrange
    GTest::GTest GTest::Main
)

gtest_discover_tests(test_numrange)
