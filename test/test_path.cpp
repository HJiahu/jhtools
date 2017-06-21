#include<iostream>
#include<string>
#include<deque>
#include<gtest/gtest.h>
#include"jhtools/path.h"

using namespace std;
using namespace jhtools;

extern string test_data_path;

class PathTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            path_ = path (test_data_path) / path ("path_test");
        }
        static void TearDownTestCase()
        {
        }
        // Some expensive resource shared by all tests.
        static path path_;
};
path PathTest::path_;



TEST_F (PathTest, string)
{
    EXPECT_EQ (path_.string(), R"(E:\jhtools\test\path_test)");
    EXPECT_NE (path_.string(), R"(E:\jhtools\test\)");
}

TEST_F (PathTest, extension)
{
    auto p1 = path_ / path ("bin/alias.bat");
    auto p2 = path_ / path ("bin/Readme.md");
    EXPECT_EQ (p1.extension(), ".bat");
    EXPECT_EQ (p2.extension(), ".md");
    EXPECT_NE (p1.extension(), "at");
    EXPECT_NE (p2.extension(), "d");
}

TEST_F (PathTest, is_absolute)
{
    EXPECT_TRUE (path_.is_absolute());
}

TEST_F (PathTest, filename)
{
    auto p1 = path_ / path ("bin/alias.bat");
    auto p2 = path_ / path ("bin/Readme.md");
    EXPECT_EQ (p1.filename(), "alias.bat");
    EXPECT_EQ (p2.filename(), "Readme.md");
    EXPECT_NE (p1.filename(), "at");
    EXPECT_NE (p2.filename(), "d");
}

TEST_F (PathTest, parent_path)
{
    EXPECT_EQ (path_.parent_path().string(), (R"(E:\jhtools\test)"));
    EXPECT_NE (path_.parent_path().string(), (R"(E:\ools\test)"));
}

TEST_F (PathTest, is_directory)
{
    EXPECT_TRUE (is_directory (path_));
    EXPECT_FALSE (is_directory (path_ / path ("bin/alias.bat")));
}

TEST_F (PathTest, is_regular_file)
{
    EXPECT_FALSE (is_regular_file (path_));
    EXPECT_TRUE (is_regular_file (path_ / path ("bin/alias.bat")));
}

TEST_F (PathTest, file_size)
{
    EXPECT_EQ (file_size (path_ / path ("icons/cmder_blue.ico")), 5272);
    EXPECT_EQ (file_size (path_ / path ("Cmder.exe")), 129536);
    EXPECT_NE (file_size (path_ / path ("icons/cmder_blue.ico")), 5272 + 200);
    EXPECT_NE (file_size (path_ / path ("Cmder.exe")), 129536 - 100);
}

TEST_F (PathTest, exists)
{
    EXPECT_FALSE (exists (path_ / path ("aaa")));
    EXPECT_TRUE (exists (path_ / path ("bin/alias.bat")));
}

TEST (path, current_path)
{
    //EXPECT_EQ(current_path(), path(R"(E:\jhtools\test\)"));
    EXPECT_NE (current_path(), path (R"(E:\ools\test\)"));
}

TEST (path, list_dir)
{
    EXPECT_EQ (4, list_dir (test_data_path + "/path_test", ListFileType::FILE).size());
    EXPECT_EQ (4, list_dir (test_data_path + "/path_test", ListFileType::DIR).size());
    EXPECT_EQ (8, list_dir (test_data_path + "/path_test", ListFileType::ALL).size());
    EXPECT_NE (10, list_dir (test_data_path + "/path_test", ListFileType::ALL).size());
    EXPECT_EQ (7, list_dir (test_data_path + "/path_test/icons", ListFileType::ALL).size());
    EXPECT_EQ (7, list_dir (test_data_path + "/path_test/icons", ListFileType::ALL).size());
    EXPECT_EQ (8, list_dir (test_data_path + "/path_test/config", ListFileType::ALL).size());
    EXPECT_EQ (1, list_dir (test_data_path + "/path_test/config", ListFileType::DIR).size());
}


