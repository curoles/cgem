// https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
//
#include "gtest/gtest.h"

TEST (BlaTest, BlaBla) {
    ASSERT_EQ (2, 2);
}

/*int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}*/
