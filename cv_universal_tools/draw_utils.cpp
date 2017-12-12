#include"draw_utils.h"
#include<memory>
#include<mutex>

namespace jhtools
{
    using namespace std;
    using namespace cv;
    
    static std::unique_ptr<CvFont> font_ptr (nullptr);
    static CvSize text_size_g;
    static mutex font_ptr_mutex;
    
    inline cv::Rect to_rect (const std::array<int, 4> &array4i)
    {
        //xmin ymin xmax ymax
        assert (array4i[0] <= array4i[2] && array4i[1] <= array4i[3]);
        return cv::Rect (array4i[0], array4i[1], array4i[2] - array4i[0], array4i[3] - array4i[1]);
    }
    
    void rectangle (cv::Mat &img,
                    const std::vector<cv::Rect> &rects,
                    cv::Scalar scalar,
                    int thickness)
    {
        for (const auto &r : rects)
        {
            cv::rectangle (img, r, scalar, thickness);
        }
    }
    
    //std::array<int, 4>
    void rectangle (cv::Mat &img,
                    const std::vector<std::array<int, 4>> &rects,
                    cv::Scalar scalar,
                    int thickness)
    {
        for (const auto &r : rects)
        {
            cv::rectangle (img, to_rect (r), scalar, thickness);
        }
    }
    void rectangle (cv::Mat &img,
                    cv::Rect rect,
                    const std::string &text,
                    const cv::Scalar &text_color,
                    const cv::Scalar &rect_color,
                    int thickness,
                    int lineType,
                    int shift)
    {
        if (font_ptr.get() == nullptr)
        {
            font_ptr_mutex.lock();
            font_ptr.reset (new CvFont());
            font_ptr_mutex.unlock();
            cvInitFont (font_ptr.get(), CV_FONT_HERSHEY_PLAIN, 1.0f, 1.0f, 0, 1, CV_AA); //设置显示的字体
            cvGetTextSize ("Hi", font_ptr.get(), &text_size_g, nullptr);
        }
        
        cv::rectangle (img, rect, rect_color, thickness, lineType, shift);
        auto ipl_img = &IplImage (img);
        cvPutText (ipl_img, text.c_str(), cvPoint (rect.x, rect.y), font_ptr.get(), text_color);
    }
    
    void rectangle (cv::Mat &img,
                    cv::Rect rect,
                    const vector<string> &text_vec,
                    const cv::Scalar &text_color,
                    const cv::Scalar &rect_color,
                    int thickness,
                    int lineType,
                    int shift)
    {
        if (font_ptr.get() == nullptr)
        {
            font_ptr_mutex.lock();
            font_ptr.reset (new CvFont());
            font_ptr_mutex.unlock();
            cvInitFont (font_ptr.get(), CV_FONT_HERSHEY_PLAIN, 1.0f, 1.0f, 0, 1, CV_AA); //设置显示的字体
            cvGetTextSize ("Hi", font_ptr.get(), &text_size_g, nullptr);
        }
        
        //如果最高行溢出了图片，就蒋所有文字转换为一行
        //cout << "***********************************" << endl;
        //for (auto &x : text_vec)cout << x << endl;
        int min_y = rect.y - text_vec.size() * text_size_g.height;
        
        if (min_y < text_size_g.height)
        {
            string buff;
            
            for (auto const&x : text_vec) { buff.append (x + " "); }
            
            rectangle (img, rect, buff, text_color, rect_color, thickness, lineType, shift);
        }
        
        else
        {
            int i = 1;
            
            for (auto x = text_vec.rbegin(); x < text_vec.rend(); x++, i++)
            {
                cv::rectangle (img, rect, rect_color, thickness, lineType, shift);
                auto ipl_img = &IplImage (img);
                cvPutText (ipl_img, x->c_str(), cvPoint (rect.x, min_y + i * text_size_g.height), font_ptr.get(), text_color);
            }
        }
    }
    
}