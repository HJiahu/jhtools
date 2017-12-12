#ifndef JH_DRAW_UTILS_H_
#define JH_DRAW_UTILS_H_

#include<array>
#include<opencv2/opencv.hpp>

namespace jhtools
{
    //********************************************************** 下面的rectangle用于在img上画方框
    typedef std::array<int, 4> array4i;
    void rectangle (cv::Mat &img,
                    const std::vector<array4i> &bbxs,
                    cv::Scalar line_color = cv::Scalar (0, 0, 255),
                    int line_width = 1);
                    
    void rectangle (cv::Mat &img,
                    const std::vector<cv::Rect> &rects,
                    cv::Scalar scalar = cv::Scalar (0, 0, 255),
                    int thickness = 1);
    void rectangle (cv::Mat &img,
                    const std::vector<std::array<int, 4>> &rects,
                    cv::Scalar scalar,
                    int thickness);
                    
    //向方框上方添加文字，一行
    void rectangle (cv::Mat &img,
                    cv::Rect rect,
                    const std::string &text,
                    const cv::Scalar &text_color = cv::Scalar (0, 255, 0),
                    const cv::Scalar &rect_color = cv::Scalar (0, 0, 255),
                    int thickness = 2,
                    int lineType = 8,
                    int shift = 0);
                    
                    
    //一般text_vec中每一个元素占一行；如果最上面那行字符已经超出了图片边缘，则用一行显示所有字符
    void rectangle (cv::Mat &img,
                    cv::Rect rect,
                    const std::vector<std::string> &text_vec,
                    const cv::Scalar &text_color = cv::Scalar (0, 255, 0),
                    const cv::Scalar &rect_color = cv::Scalar (0, 0, 255),
                    int thickness = 2,
                    int lineType = 8,
                    int shift = 0);
}

#endif // !JH_DRAW_UTILS_H_
