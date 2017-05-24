#include<gtest/gtest.h>
#include<string>
#include"jhtools/directory.h"

using namespace std;
using namespace jhtools;

static const string xml_path("E:/jhtools/test/xml_files");

class DirectoryTest :public testing::Test{
protected:
	static void SetUpTestCase(){

	}
	static void TearDownTestCase(){

	}

	static Directory dir_;
};
Directory DirectoryTest::dir_(xml_path);

TEST_F(DirectoryTest, has_file){
	EXPECT_TRUE(dir_.has_file("mov_001_007708.xml"));
	EXPECT_FALSE(dir_.has_file("mov_00_007708.xml"));

	EXPECT_FALSE(dir_.has_file(".")); //. and .. are directories
	EXPECT_FALSE(dir_.has_file(".."));
}

TEST_F(DirectoryTest, sub_dirs){
	EXPECT_EQ(2, dir_.count_all() - dir_.count_regular_file());
}

TEST_F(DirectoryTest, regular_files){
	EXPECT_EQ(10, dir_.regular_files()->size());
	EXPECT_TRUE(dir_.regular_files()->operator[](0).rfind(".xml"));
	EXPECT_EQ(9, dir_.regular_files("xml")->size());
}


TEST_F(DirectoryTest, count){
	EXPECT_EQ(10 + 2, dir_.count_all());
	EXPECT_EQ(10, dir_.count_regular_file());
}
