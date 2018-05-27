add_executable (test_ct_math
    test_ct_math.cpp
)

target_link_libraries(test_ct_math
    GTest::GTest GTest::Main
)

gtest_discover_tests(test_ct_math)
