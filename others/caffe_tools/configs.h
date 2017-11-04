#ifndef CAFFE_TOOLS_CONFIG_H_
#define CAFFE_TOOLS_CONFIG_H_
#include<cassert>
#include<string>
#include"jhtools/path.h"
#include"jhtools/ezlog.h"
#include"jhtools/utils.h"

//从json文件中解析而来的数据都将保存在下面的变量中
extern jhtools::Json configs_json_g;
//action指定了当前函数的行为，不同的action当前函数将有不同的行为，也会使用json中不同的数据项
extern  std::string action_g;
//caffe模型路径可以使用两种方式指定，separate和single
//如果path_mode_g是single则model_path_all_in_g中需要包含所有caffe需要的四个文件，并要满足命名规则
extern  std::string path_mode_g;
extern  jhtools::path model_path_all_in_g;
extern int prompt_count_g;
//声明全局的caffe分类对象
class Classifier;
extern Classifier* classifier_ptr_g;

void init_env (const jhtools::path & config_file_path);
#endif