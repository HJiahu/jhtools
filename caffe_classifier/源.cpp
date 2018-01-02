#include<string>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<jheaders/filesystem.h>

namespace jh = jheaders;
using namespace std;

int main()
{
    string img_path{R"(D:\MyPaperData\HeadsData\heads)"};
    auto file_names = jh::list_dir (img_path, ".jpg");
    
    for (auto &s : file_names)
    {
        auto path = jh::Path (img_path) / s;
        cv::Mat img = cv::imread (path.string());
        
        if (img.empty())
        {
            EZLOG_ (fatal) << "can not open this file: " << path.string();
        }
        
        else
        {
            if (img.cols >= 90)
            {
				jh::copy
            }
        }
    }
}