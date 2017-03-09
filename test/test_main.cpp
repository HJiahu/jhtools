#include<gtest/gtest.h>
#include<utils/termcolor.hpp>
#include<iostream>
#include<ezlog.h>
#include<file.h>
using namespace std;

int main(int argc, char ** argv){
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
}
