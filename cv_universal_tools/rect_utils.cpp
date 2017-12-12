#include"rect_utils.h"

namespace jhtools
{
    inline cv::Rect verify_rect (const cv::Rect& rect, const cv::Mat &img)
    {
        cv::Rect new_rect (rect.x >= 0 ? rect.x : 0, rect.y >= 0 ? rect.y : 0, rect.width, rect.height);
        
        if (img.empty()) { return new_rect; }
        
        else
        {
            if (new_rect.width + new_rect.x > img.cols) { new_rect.width = img.cols - new_rect.x; }
            
            if (new_rect.height + new_rect.y > img.rows) { new_rect.height = img.rows - new_rect.y; }
            
            new_rect.width = new_rect.width >= 0 ? new_rect.width : 0;
            new_rect.height = new_rect.height >= 0 ? new_rect.height : 0;
        }
        
        return new_rect;
    }
    
    
    cv::Rect resize_rect (const cv::Rect &rect, double scale, const cv::Mat &img)
    {
        assert (scale > 0);
        cv::Rect new_rect = rect;
        
        //if 1.0 == scale
        if (fabs (1.0 - scale) <= FLT_EPSILON) { return verify_rect (rect, img); }
        
        else
            if (scale > 1.0)
            {
                int width_dif = int (0.5 * rect.width * (scale - 1));
                int height_dif = int (0.5 * rect.height * (scale - 1));
                new_rect.x = rect.x - width_dif;
                new_rect.y = rect.y - height_dif;
                new_rect.width = int (rect.width * scale);
                new_rect.height = int (rect.height * scale);
            }
            
            else
                if (scale < 1.0)
                {
                    int width_dif = int (0.5 * rect.width * (1 - scale));
                    int height_dif = int (0.5 * rect.height * (1 - scale));
                    cv::Rect new_rect;
                    new_rect.x = rect.x + width_dif;
                    new_rect.y = rect.y + height_dif;
                    new_rect.width = int (rect.width * scale);
                    new_rect.height = int (rect.height * scale);
                }
                
        if (! ( (rect & cv::Rect (0, 0, img.cols, img.rows)) == rect) || img.empty())
        {
            return verify_rect (rect, img);
        }
        
        else { return verify_rect (new_rect, img); }
    }
}