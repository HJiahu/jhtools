#include<gtest/gtest.h>
#include<string>
#include<directory.h>

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

	EXPECT_TRUE(dir_.has_file(L"mov_001_007708.xml"));
	EXPECT_TRUE(dir_.has_file(L"¿Ÿƒ».jpeg"));
	//EXPECT_TRUE(dir_.has_file(".")); //. and .. are directories
	//EXPECT_TRUE(dir_.has_file(".."));
}

TEST_F(DirectoryTest, sub_dirs){
	EXPECT_EQ(2, dir_.count_all() - dir_.count_regular_file());
}

TEST_F(DirectoryTest, count){
	EXPECT_EQ(10+2, dir_.count_all());
	EXPECT_EQ(10, dir_.count_regular_file());
}
