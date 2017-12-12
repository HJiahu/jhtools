#ifndef CAFFE_TOLLS_ACTIONS_
#define CAFFE_TOLLS_ACTIONS_
#include<string>
#include<map>
#include<memory>
#include<algorithm>
#include<jheaders/json.h>

namespace caffe_tool_actions
{
    //每一个行为都继承于下面的类
    class Action
    {
        public:
            Action (const std::string& action_str, const std::map<std::string, std::string>&config)
                : action_str_ (action_str), config_ (config) {}
            const std::string& action_str() const { return action_str_; }
            virtual void execute() const = 0; //执行具体的行为
            void operator() () const
            {
                return execute();
            }
            void init_classifier (const std::string&)
        protected:
            std::string action_str_;//描述当前对象的行为
            std::map<std::string, std::string> config_;
            std::vector<void*> classifiers_ptr_;
    };
    //使用反射生成一个action对象，不考虑多线程的使用所以这里不考虑互斥
    class ActionFactory final
    {
        public:
            static Action* create_action (const std::string& action_str);
            
            static void regist (std::shared_ptr<Action> action_ptr);
        private:
            //每一个写成的action对象都将在这个函数中注册到action工厂
            static void regist_all();
        private:
            static std::map < std::string, std::shared_ptr<Action> > actions_;
    };
    
    
    
    //下面的行为对图片进行分类并显示分类结果
    class ImgClassifier final : public Action
    {
        public:
            using Action::Action;
            void execute() const override;
    };
    //对一个文件夹下的图片进行分类并显示分类结果
    class DirImgsClassifier final : public Action
    {
        public:
            using Action::Action;
            void execute() const override;
    };
    
    //下面的行为用于获得模型在当前环境下执行的时间
    class ModelSpeed final: public Action
    {
        public:
            using Action::Action;
            void execute() const override;
    };
    
    
#endif // !CAFFE_TOLLS_ACTIONS_
    
    
}//caffe_tool_actions