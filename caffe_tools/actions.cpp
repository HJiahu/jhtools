#include<opencv2/opencv.hpp>
#include"actions.h"
#include<iostream>
#include<sstream>
#include<chrono>
#include<jheaders/filesystem.h>

using namespace std;
namespace jh = jheaders;

namespace caffe_tool_actions
{
    inline string caffe_infos()
    {
        ostringstream infos;
        
        if (path_mode_g == "single")
        {
            infos << "caffe model path: " << configs_json_g["model_path_all_in"].string_value() + "model.caffemodel" << endl;
        }
        
        else
        {
            infos << "caffe model path: " << configs_json_g["model_file_path"].string_value() << endl;
        }
        
        infos << "caffe model: " << configs_json_g["caffe_model"].string_value() << endl;
        return infos.str();
    }
    std::map < std::string, std::shared_ptr<Action> > ActionFactory::actions_;
    void ModelSpeed::execute() const
    {
        jh::path img_path (configs_json_g["img_path"].string_value());
        int repetition_count = configs_json_g["repetition_count"].int_value();
        cv::Mat img (cv::imread (img_path.string()));
        
        if (img.empty())
        {
            EZLOG (jh::Log_level::FATAL) << "Can not open this file: " << img_path.string();
        }
        
        assert (repetition_count > 0);
        cout << "test model speed in this PC. " << endl;
        cout << "img path: " << img_path.string() << endl;
        cout << "repetition_count: " << repetition_count << endl;
        cout << caffe_infos();
        cout << "doing";
        chrono::system_clock::time_point start = chrono::system_clock::now();
        
        for (int i = 0; i < repetition_count; i++)
        {
            classifier_ptr_g->Classify (img, configs_json_g["category_num"].int_value());
            
            if (i % prompt_count_g == 0) { cout << "."; }
        }
        
        chrono::system_clock::time_point end = chrono::system_clock::now();
        cout << endl;
        chrono::microseconds micro_secs = chrono::duration_cast<chrono::microseconds> (end - start);
        cout << "Time consumption: " << micro_secs.count() << " microseconds(not ms!)." << endl;
        cout << "( probably " << chrono::duration_cast<chrono::milliseconds> (micro_secs).count() << "ms or "\
             << "probaly " << chrono::duration_cast<chrono::seconds> (micro_secs).count() << "s. )" << endl;
    }
    
    
    
    
    void ImgClassifier::execute() const
    {
        jh::path img_path (configs_json_g["img_path"].string_value());
        cv::Mat img (cv::imread (img_path.string()));
        
        if (img.empty())
        {
            EZLOG (jh::Log_level::FATAL) << "Can not open this file: " << img_path.string();
        }
        
        else
        {
            cout << "classify img " << endl;
            cout << "img path: " << img_path.string() << endl;
            cout << caffe_infos();
            cout << "doing" << endl;
            auto result = classifier_ptr_g->Classify (img, configs_json_g["category_num"].int_value());
            cout << "\n*********   predict results:   *********" << endl;
            for_each (result.begin(), result.end(), [] (const Prediction & p)->void {cout << p << endl; });
        }
    }
    
    void DirImgsClassifier::execute() const
    {
        jh::path img_dir (configs_json_g["img_dir"].string_value());
        auto files = list_dir (img_dir);
        map<string, int> results;
        int process_count = 0;
        cout << "classifying imgs in dir." << endl;
        cout << caffe_infos();
        cout << "doing";
        
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
                results[pre[0].first]++;
                process_count++;
                
                if (process_count % prompt_count_g == 0) { cout << "." << fflush; }
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
        /******************** 所有action注册到这里才会生效 ********************/
        regist (std::shared_ptr<Action> (new DirImgsClassifier ("classify_img_in_dir")));
        regist (std::shared_ptr<Action> (new ImgClassifier ("classify_one_img")));
        regist (std::shared_ptr<Action> (new ModelSpeed ("test_time_consumption")));
        //regist(std::shared_ptr<Action>(new ImgClassifier("classify_one_img")));
        //regist(std::shared_ptr<Action>(new ImgClassifier("classify_one_img")));
        /***************************************/
        //disply infos
        cout << "\n (O__O\")  Actions can be used  (O__O\") " << endl;
        int count = 1;
        
        for (auto const&item : actions_)
        {
            cout << count++ << ". " << item.first << ";" << endl;
        }
        
        cout << "\n            (O__O\")              \n" << endl;
    }
    
    Action* ActionFactory::create_action (const std::string& action_str)
    {
        if (actions_.size() == 0) { regist_all(); }
        
        Action* ptr = nullptr;
        
        for (auto itr = actions_.begin(); itr != actions_.end(); itr++)
        {
            if (itr->first == action_str)
            {
                auto x = itr->second.get();
                return itr->second.get();
            }
        }
        
        EZLOG (jh::Log_level::ERR) << "no such action: " << action_str;
        return nullptr;
    }
    
    void ActionFactory::regist (std::shared_ptr<Action> action_ptr)
    {
        if (action_ptr.get() == nullptr || action_ptr->action_str().size() == 0)
        {
            EZLOG (jh::Log_level::FATAL) << "can not regist this action: " \
                                         << action_ptr->action_str() \
                                         << " pointer: " << action_ptr.get();
        }
        
        else
        {
            //有些key已经存在了就不能再用不然会发生歧义
            if (actions_.find (action_ptr->action_str()) != actions_.end())
            {
                EZLOG (jh::Log_level::FATAL) << "resue key: " << action_ptr->action_str();
            }
            
            else
            {
                actions_.insert (std::make_pair (action_ptr->action_str(), action_ptr));
            }
        }
    }
}

