#ifndef CAFFE_CLASSFIER_H_
#define CAFFE_CLASSFIER_H_

#include <string>
#include <utility>
#include <vector>
#include <ostream>
#include <mutex>
#include <opencv2/opencv.hpp>


using std::string;

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;
inline std::ostream& operator<< (std::ostream& out, const Prediction&pred)
{
    out << "[" << pred.first << ", " << pred.second * 100.0 << "%]";
    return out;
}
static bool PairCompare (const std::pair<float, int>& lhs,
                         const std::pair<float, int>& rhs);
/* Return the indices of the top N values of vector v. */
static std::vector<int> Argmax (const std::vector<float>& v, int N);

/* Return the indices of the top N values of vector v. */
std::vector<int> Argmax (const std::vector<float>& v, int N);

class Classifier
{
    public:
        Classifier (const string& model_file,
                    const string& trained_file,
                    const string& mean_file,
                    const string& label_file);
        //使用约定的方式来初始化classifier，规定文件夹内文件命名方式如下：
        //model_file   :deploy.prototxt
        //trained_file :model.caffemodel
        //mean_file    :mean.binaryproto
        //label_file   :label.txt
        explicit Classifier (const string& cnn_model_path);
        ~Classifier();
        Classifier (const Classifier&) = delete;
        Classifier (Classifier&&) = delete;
        Classifier&operator = (const Classifier&) = delete;
        Classifier&operator = (Classifier&&);
        std::vector<Prediction> Classify (const cv::Mat& img, int N = 2);
        
    private:
        void SetMean (const string& mean_file);
        
        std::vector<float> Predict (const cv::Mat& img);
        
        void WrapInputLayer (std::vector<cv::Mat>* input_channels);
        
        void Preprocess (const cv::Mat& img, std::vector<cv::Mat>* input_channels);
        
        
    private:
        //std::shared_ptr<caffe::Net<float> > net_;
        void *net_;
        cv::Size input_geometry_;
        int num_channels_;
        cv::Mat mean_;
        std::vector<string> labels_;
};

//pool中的每一个分类器都有一个对应的mutex
class ClassifierPool
{
    public:
        //第二个参数表示分类器的个数
        ClassifierPool (const std::string &caffe_model_path, int num);
        Classifier& get_classifier();
    private:
        std::vector<Classifier> classifiers_;
        std::vector<std::mutex> mutex_vec_;
};
inline bool PairCompare (const std::pair<float, int>& lhs,
                         const std::pair<float, int>& rhs)
{
    return lhs.first > rhs.first;
}



#endif // !CAFFE_CLASSFIER.H


