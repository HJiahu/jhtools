#include<gtest/gtest.h>
#include<iostream>
#include"jhtools/ezlog.h"
#include"jhtools/path.h"
#include"jhtools/termcolor.hpp"
using namespace std;
using namespace jhtools;
#define TEST_JHTOOLS

#ifdef TEST_JHTOOLS
int main(int argc, char ** argv){
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	
	system("pause");
}
#else
int main(){
	path p(R"(E:\jhtools\test\path_test)");
	cout << p.parent_path().string()<<endl;
	system("pause");
}
#endif