#include<gtest/gtest.h>
#include<iostream>
#include<jhtools.h>
#include<jhtools/ezlog.h>
using namespace std;
using namespace jhtools;

#if defined(_WIN32)
    string test_data_path (R"(E:\jhtools\test)");
#else
    
#endif


#define TEST_JHTOOLS

#ifdef TEST_JHTOOLS
int main (int argc, char ** argv)
{
    if (!path (test_data_path).exists())
    {
        EZLOG (Log_level::FATAL) << "test path is not exists" << test_data_path;
    }
    
    testing::InitGoogleTest (&argc, argv);
    RUN_ALL_TESTS();
    system ("pause");
}
#else
int main()
{
    string j_str (R"({
"china":960,
"canada":998
}
)");
	auto jn = stojson(j_str);
	system("pause");
}
#endif