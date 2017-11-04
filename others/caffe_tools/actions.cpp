#include<opencv2/opencv.hpp>
#include<jhtools/others/caffe_classifier.h>
#include"actions.h"
#include<iostream>
#include"jhtools/json11.h"
#include"jhtools/ezlog.h"
#include"jhtools/utils.h"
using namespace std;
using namespace jhtools;
namespace caffe_tool_actions
{
    std::map < std::string, std::shared_ptr<Action> > ActionFactory::actions_;
    void ModelSpeed::execute() const
    {
    }
    
    void ImgClassifier::execute() const
    {
        jhtools::path img_path (configs_json_g["img_path"].string_value());
        cv::Mat img (cv::imread (img_path.string()));
        
        if (img.empty())
        {
            EZLOG (jhtools::Log_level::FATAL) << "Can not open this file: " << img_path.string();
        }
        
        else
        {
            auto result = classifier_ptr_g->Classify (img, configs_json_g["category_num"].int_value());
            cout << "\n*********   predict results:   *********" << endl;
            for_each (result.begin(), result.end(), [] (const Prediction & p)->void {cout << p << endl; });
        }
    }
    
    void DirImgsClassifier::execute() const
    {
        jhtools::path img_dir (configs_json_g["img_dir"].string_value());
        auto files = list_dir (img_dir);
        map<string, int> results;
        int process_count = 0;
        
        for (const auto &f : files)
        {
            auto full_path = img_dir / f;
            cv::Mat img (cv::imread (full_path.string()));
            
            if (img.empty())
            {
                EZLOG (Log_level::FATAL) << "Can not open this file: " << full_path.string();
            }
            
            else
            {
                auto pre = classifier_ptr_g->Classify (img, configs_json_g["category_num"].int_value());
                
                for (auto x : pre)
                {
                    results[x.first]++;
                }
                
                process_count++;
                
                if (process_count % 100 == 0) { cout << "."; }
            }
        }
        
        cout << endl;
        unsigned int img_count = 0;
        for_each (results.begin(), results.end(), [&] (const pair<string, int> & p) {img_count += p.second; });
        cout << "\n*********   predict results:   *********" << endl;
        cout << "img_count: " << img_count << endl;
        for_each (results.begin(), results.end(), [ = ] (const pair<string, int> & p)
        {
            cout << p.first << "\t: " << p.second << "  (" << 100.0 * p.second / img_count << "%)" << endl;
        });
    }
    
    void ActionFactory::regist_all()
    {
        /******************** 只有写好的action注册到这里才会生效 ********************/
        regist (std::shared_ptr<Action> (new DirImgsClassifier ("classify_img_in_dir")));
        regist (std::shared_ptr<Action> (new ImgClassifier ("classify_one_img")));
        /***************************************/
        //disply infos
        cout << "\n ︿(￣︶￣)︿  Actions can be used  ︿(￣︶￣)︿" << endl;
        int count = 1;
        
        for (auto const&item : actions_)
        {
            cout << count++ << ". " << item.first << ";" << endl;
        }
        
        cout << "\n               ︿(￣︶￣)︿                    \n" << endl;
    }
}