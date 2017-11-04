#include"jhtools/others/caffe_classifier.h"
#include"configs.h"

//从json文件中解析而来的数据都将保存在下面的变量中
jhtools::Json configs_json_g;
//action指定了当前函数的行为，不同的action当前函数将有不同的行为，也会使用json中不同的数据项
std::string action_g;
//caffe模型路径可以使用两种方式指定，separate和single
//如果path_mode_g是single则model_path_all_in_g中需要包含所有caffe需要的四个文件，并要满足命名规则
std::string path_mode_g;
jhtools::path model_path_all_in_g;
Classifier* classifier_ptr_g;

void init_env (const jhtools::path & config_file_path)
{
    if (!jhtools::exists (config_file_path) || config_file_path.extension() != ".json")
    {
        EZLOG (jhtools::Log_level::FATAL) << "config file error: " << config_file_path.string();
    }
    
    else
    {
        configs_json_g = jhtools::ftojson (config_file_path.string());
        //init path_mode
        path_mode_g = configs_json_g["path_mode"].string_value();
        //init model_path_all_in_g
        model_path_all_in_g = configs_json_g["model_path_all_in"].string_value();
        //init separate path for caffe
        auto model_file_path = configs_json_g["model_file_path"].string_value();
        auto trained_file_path = configs_json_g["trained_file_path"].string_value();
        auto mean_file_path = configs_json_g["mean_file_path"].string_value();
        auto label_file_path = configs_json_g["label_file_path"].string_value();
        
        if (path_mode_g == "single")
        {
            classifier_ptr_g = new Classifier (model_path_all_in_g.string());
        }
        
        else
            if (path_mode_g == "separate")
            {
                classifier_ptr_g = new Classifier (model_file_path,
                                                   trained_file_path,
                                                   mean_file_path,
                                                   label_file_path);
            }
    }
}