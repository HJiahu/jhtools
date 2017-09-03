#include<algorithm>
#include<map>
#include<numeric>
#include"jhtools/others/caffe_classifier.h"
#include"jhtools/path.h"
#include"jhtools/json11.h"
#include"jhtools/ezlog.h"
#include"jhtools/utils.h"
#include"configs.h"


using namespace jhtools;
using namespace std;

int main (int argc, char** argv)
{
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
    Classifier classifier (model_file_path_g.string(),
                           trained_file_path_g.string(),
                           mean_file_path_g.string(),
                           label_file_path_g.string());
                           
    if (file_type_g == "FILE")
    {
        cv::Mat img (cv::imread (file_path_g.string()));
        
        if (img.empty())
        {
            EZLOG (Log_level::FATAL) << "Can not open this file: " << file_path_g.string();
        }
        
        else
        {
            auto result = classifier.Classify (img);
            cout << "\n*********   predict results:   *********" << endl;
            for_each (result.begin(), result.end(), [] (const Prediction & p) {cout << p << endl; });
        }
    }
    
    if (file_type_g == "DIR")
    {
        auto files = list_dir (file_path_g);
        map<string, int> results;
        int process_count = 0;
        
        for (const auto &f : files)
        {
            auto full_path = file_path_g / f;
            cv::Mat img (cv::imread (full_path.string()));
            
            if (img.empty())
            {
                EZLOG (Log_level::FATAL) << "Can not open this file: " << full_path.string();
            }
            
            else
            {
                auto pre = classifier.Classify (img);
                results[pre[0].first]++;
                process_count++;
                
                if (process_count % 100 == 0) { cout << "."; }
            }
        }
        
        cout << endl;
        unsigned int img_count = 0;
        for_each (results.begin(), results.end(), [&] (const pair<string, int> & p) {img_count += p.second; });
        cout << "\n*********   predict results:   *********" << endl;
        cout << "img_count: " << img_count << endl;
        for_each (results.begin(), results.end(), [ = ] (const pair<string, int> & p) {cout << p.first << "\t: " << p.second << "  (" << 100.0 * p.second / img_count << "%)" << endl; });
    }
    
#ifdef _MSC_VER
    system ("pause");
#endif // __MSC_VER
}