add_executable (test_cpu_cache
    test_cpu_cache.cpp
)

target_link_libraries(test_cpu_cache
    GTest::GTest GTest::Main
)

gtest_discover_tests(test_cpu_cache)
