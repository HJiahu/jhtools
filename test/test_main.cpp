#include<gtest/gtest.h>
#include"jhtools/utils/termcolor.hpp"
#include<iostream>
#include"jhtools/ezlog.h"
#include"jhtools/path.h"
using namespace std;

int main(int argc, char ** argv){
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	
	system("pause");
}
