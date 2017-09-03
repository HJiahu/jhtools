#ifndef CAFFE_TOOLS_CONFIG_H_
#define CAFFE_TOOLS_CONFIG_H_
#include<cassert>
#include<string>
#include"jhtools/path.h"
#include"jhtools/ezlog.h"
#include"jhtools/utils.h"
static jhtools::path model_file_path_g;
static jhtools::path trained_file_path_g;
static jhtools::path mean_file_path_g;
static jhtools::path label_file_path_g;
static std::string file_type_g;//有两种选择：FILE、DIR
static jhtools::path file_path_g;

inline void init_env (const jhtools::path & config_file_path)
{
    if (!jhtools::exists (config_file_path) || config_file_path.extension() != ".json")
    {
        EZLOG (jhtools::Log_level::FATAL) << "config file error: " << config_file_path.string();
    }
    
    else
    {
        auto jn = jhtools::ftojson (config_file_path.string());
        model_file_path_g = jn["model_file_path"].string_value();
        trained_file_path_g = jn["trained_file_path"].string_value();
        mean_file_path_g = jn["mean_file_path"].string_value();
        label_file_path_g = jn["label_file_path"].string_value();
        file_type_g = jn["file_type"].string_value();
        file_path_g = jn["file_path"].string_value();
    }
    
    assert (model_file_path_g.length() *trained_file_path_g.length() *mean_file_path_g.length() *label_file_path_g.length() != 0);
}

#endif