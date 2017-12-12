//因为caffe依赖库的原因，最好经caffe头文件放在所有头文件之前
#include"jhtools/others/caffe_classifier.h"
#include<algorithm>
#include<map>
#include<numeric>
#include"jhtools/path.h"
#include"configs.h"
#include"actions.h"


using namespace jhtools;
using namespace std;
using namespace caffe_tool_actions;
/*********************************************
	当前项目的功能如下：
	①、测试网络在当前环境下的速度，设定mode为test_speed；指定测试图片的路径；指定测试的次数
**********************************************/

int main (int argc, char** argv)
{
    //::google::InitGoogleLogging (argv[0]);
    //默认情况下使用当前文件夹下的init.json来初始化整个环境
    jhtools::path config_file_path ;

    if (argc == 1)
    {
        config_file_path = "./init.json";
    }

    else
        if (argc == 2)
        {
            config_file_path = std::string (argv[1]);
            EZLOG (Log_level::INFO) << "Loading configs from :" << config_file_path.string();
        }

        else
        {
            EZLOG (Log_level::INFO) << "usage: execute_file config_file.json";
            getchar();
            exit (0);
        }

    init_env (config_file_path);
    Action* action_ptr = ActionFactory::create_action (configs_json_g["action"].string_value());
    action_ptr->execute();
#ifdef _MSC_VER
    system ("pause");
#endif // __MSC_VER
}