#include "cppmagic/FilePath.h"
#include <gtest/gtest.h>

TEST(FilePathTest, SampleTest)
{
    FilePath path("../include/cppmagic/TagInfo.h");
    path.print();
    EXPECT_EQ(1, 1);
}
