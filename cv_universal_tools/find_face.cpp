#include"find_face.h"
#include<jheaders/filesystem.h>

namespace jhtools
{
    namespace jh = jheaders;
    using namespace std;
    
    std::vector<cv::Rect> find_face_by_haarlike (const cv::Mat& img, const std::string& haarcascades_path, cv::Size min_size, cv::Size max_size)
    {
        if (!jh::exists (haarcascades_path))
        {
            EZLOG_ (fatal) << "can not open this file: " << haarcascades_path;
        }
        
        vector<cv::Rect> faces_vec;
        
        if (img.empty()) { return faces_vec; }
        
        else
        {
            cv::CascadeClassifier ccf;
            
            if (!ccf.load (haarcascades_path)) //╪стья╣а╥нд╪Ч
            {
                EZLOG_ (fatal) << "can not load haarcascades file";
            }
            
            cv::Mat gray;
            cvtColor (img, gray, CV_BGR2GRAY);
            cv::equalizeHist (gray, gray);
            ccf.detectMultiScale (gray, faces_vec, 1.1, 3, 0, min_size, max_size);
            return faces_vec;
        }
    }
}

