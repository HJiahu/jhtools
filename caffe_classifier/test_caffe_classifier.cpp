//#defien USE_THIS_FILE
#ifdef USE_THIS_FILE


#include"caffe_classifier.h"
#include<jheaders/filesystem.h>
#include<jheaders/string_utils.h>
#include<jheaders/time_utils.h>
#include<string>
#include<opencv2/opencv.hpp>

using namespace std;
namespace jh = jheaders;
namespace jt = jhtools;

int main()
{
    const std::string caffe_modle_path{ "D:/age_gender/deepid/gender" };
    jt::Classifier classifier (caffe_modle_path);
    const std::string test_img_path{ "D:/face_data/sample2000" };
    auto files = jh::list_dir (test_img_path);
    int millis;
    int cnt = 0;
    TIME_CONSUME_MICRO_SECS (millis,
    
                             for (auto &f : files)
{
    if (++cnt == 1000) { break; }
    
        auto file_path = jh::Path (test_img_path) / f;
        auto splits = jh::split (f, '_');
        cv::Mat img{ cv::imread (file_path.string()) };
        auto pres = classifier.Classify (img);
        //cout << splits[1] << " " << pres[0].first << " " << pres[0].second << endl;
    });
    cout << millis << endl;
    system ("pause");
}

#endif // USE_THIS_FILE

