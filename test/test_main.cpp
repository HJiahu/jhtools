#include<gtest/gtest.h>
#include<iostream>
#include<jhtools.h>
using namespace std;
using namespace jhtools;
#define TEST_JHTOOLS

#ifdef TEST_JHTOOLS
int main (int argc, char ** argv)
{
    testing::InitGoogleTest (&argc, argv);
    RUN_ALL_TESTS();
    system ("pause");
}
#else
int main()
{

	system("pause");
}
#endif