#include<vector>
#include<string>
#include<opencv2/opencv.hpp>
#include<jheaders/3rdparty/rapidxml/rapidxml.hpp>

namespace jhtools
{
    struct BoundBox
    {
        int x_min_;
        int x_max_;
        int y_min_;
        int y_max_;
        BoundBox (int x_min, int x_max, int y_min, int y_max)
            : x_min_ (x_min), x_max_ (x_max), y_min_ (y_min), y_max_ (y_max) {}
    };
    
    std::vector<BoundBox> bbx_in_xml (const std::string& path)
    {

    }
}