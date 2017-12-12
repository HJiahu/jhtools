//#define USE_THIS_FILE
#ifdef USE_THIS_FILE

#include"find_face.h"
#include"draw_utils.h"
#include<jheaders/ezlog.h>

namespace jh = jheaders;
using namespace jhtools;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

int main()
{
    std::string haar_xml{"D:/Programs/Libs/opencv310_vs2015x64_win10/etc/haarcascades/haarcascade_frontalface_default.xml"};
    cv::VideoCapture cap (0);
    cap.set (CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    cap.set (CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    
    if (!cap.isOpened())
    {
        EZLOG_ (fatal) << "could not open camera";
    }
    
    cv::Mat frame;
    
    while (cap.read (frame))
    {
        auto faces = find_face_by_haarlike (frame, haar_xml);
        rectangle (frame, faces);
        cv::imshow ("test", frame);
        cv::waitKey (1);
    }
}

#endif // USE_THIS_FILE