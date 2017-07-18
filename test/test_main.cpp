#include<gtest/gtest.h>
#include<iostream>
#include<jhtools.h>
#include<jhtools/ezlog.h>
#include<jhtools/utils.h>
using namespace std;
using namespace jhtools;

#if defined(_WIN32)
    string test_data_path (R"(G:\jhtools\test)");
#else
    string test_data_path ("./");
    
#endif


//#define TEST_JHTOOLS

#ifdef TEST_JHTOOLS
int main (int argc, char ** argv)
{
    if (!path (test_data_path).exists())
    {
        EZLOG (Log_level::FATAL) << "test path is not exists" << test_data_path;
    }
    
    testing::InitGoogleTest (&argc, argv);
    RUN_ALL_TESTS();
#if defined(_WIN32)
    system ("pause");
#endif
}
#else



int main()
{
    cout << make_json ({ { "china", 960 }, { "russia", "1707" }, { "america", 936 } }).dump() << endl;
#ifdef _MSC_VER
    system ("pause");
#endif
}
#endif
