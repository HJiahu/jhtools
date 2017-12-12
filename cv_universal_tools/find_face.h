#ifndef JH_FIND_FACE_H_
#define JH_FIND_FACE_H_

#include<vector>
#include<string>
#include "opencv2\opencv.hpp"

namespace jhtools
{
    //D:/Programs/Libs/opencv310_vs2015x64_win10/etc/haarcascades/haarcascade_frontalface_default.xml
    std::vector<cv::Rect> find_face_by_haarlike (const cv::Mat& img, const std::string& haarcascades_path,
            cv::Size min_size = cv::Size (30, 30), cv::Size max_size = cv::Size (400, 400));
}

#endif // !JH_FIND_FACE_H_


