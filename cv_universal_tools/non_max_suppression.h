#ifndef JH_NON_MAX_SUPPRESSION_H_
#define JH_NON_MAX_SUPPRESSION_H_

#include <list>
#include <vector>
#include <opencv2/opencv.hpp>

namespace jhtools
{
    //如果两个方框相交的面积比任何一个框面积的比大于threshold则删除其中一个
    inline std::vector<cv::Rect> non_max_suppression (const std::vector<cv::Rect>& bboxs, double threshold, bool delete_smaller = true)
    {
        assert (threshold > 0.0 && threshold < 1.0);
        std::vector<cv::Rect> rest;
        std::list<cv::Rect> bboxs_buff;
        
        for (auto r : bboxs) { bboxs_buff.push_back (r); }
        
        while (bboxs_buff.size() > 0)
        {
            auto curr_best = bboxs_buff.front();
            bboxs_buff.erase (bboxs_buff.begin());
            
            for (auto itr = bboxs_buff.begin(); itr != bboxs_buff.end();)
            {
                double overlap_area = ( (*itr) & (curr_best)).area();
                //为了防止面积相等时bigger和smaller引用同一个对象，这里使用不同的顺序
                auto &bigger = (*itr).area() >= curr_best.area() ? *itr : curr_best;
                auto &smaller = (*itr).area() >= curr_best.area() ? curr_best : *itr;
                
                if (overlap_area / curr_best.area() >= threshold || overlap_area / itr->area() >= threshold)
                {
                    if (delete_smaller)
                    {
                        curr_best = bigger;
                        itr = bboxs_buff.erase (itr);
                    }
                    
                    else
                    {
                        curr_best = smaller;
                        itr = bboxs_buff.erase (itr);
                    }
                }
                
                else
                {
                    itr++;
                }
            }
            
            rest.push_back (curr_best);
        }
        
        return rest;
    }
}

#endif // !JH_NON_MAX_SUPPRESSION_H_
