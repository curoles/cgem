// https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
//
#include "gtest/gtest.h"
#include "cgem/stack_direction.h"
#include "cgm/stack_direction.h"

TEST (StackDirection, C_test) {
    ASSERT_EQ (cgm__stack_direction(), -1) << "Stack on x86 grows downward";
}

TEST (StackDirection, Cpp_test) {
    ASSERT_EQ (cgm::stack_direction(),
               cgm::StackDir::Down) << "Stack on x86 grows downward";
}

/*int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}*/
