add_executable (test_stack_direction
    test_stack_direction.cpp
)

target_link_libraries(test_stack_direction
    GTest::GTest GTest::Main
    StackDirection
)
#target_link_libraries(test_stack_direction
#    ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread
#)

#add_test(
#    NAME TestStackDirection
#    COMMAND test_stack_direction
#)

gtest_discover_tests(test_stack_direction)
