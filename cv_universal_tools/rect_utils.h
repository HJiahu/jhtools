#ifndef JH_RECT_UTILS_H_
#define JH_RECT_UTILS_H_

#include<iostream>
#include<opencv2/opencv.hpp>


namespace jhtools
{
    //判断rect是否位于img中
    inline cv::Rect verify_rect (const cv::Rect& rect, const cv::Mat &img = cv::Mat());
    
    //放大rect，rect = rect*scale，前提是rect依旧位于img内，如果不满足这个要求rect将保持原样并返回false
    //如果img为空则不做rect的合法性检测
    cv::Rect resize_rect (const cv::Rect&rect, double scale, const cv::Mat&img = cv::Mat());
}

#endif // !JH_RECT_UTILS_H_
