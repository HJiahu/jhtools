#ifndef CAFFE_TOLLS_ACTIONS_
#define CAFFE_TOLLS_ACTIONS_
#include<string>
#include<map>
#include<memory>
#include<algorithm>
#include"configs.h"

namespace caffe_tool_actions
{
	//每一个行为都继承于下面的类
    class Action
    {
        public:
            Action (const std::string& action_str, const jhtools::Json& config_json = configs_json_g)
                : action_str_ (action_str), config_json_ (config_json) {}
            const std::string& action_str() const { return action_str_; }
            virtual void execute() const = 0; //执行具体的行为
            void operator() () const
            {
                return execute();
            }
        protected:
            std::string action_str_;//描述当前对象的行为
            const jhtools::Json& config_json_;
    };
    //使用反射生成一个action对象，不考虑多线程的使用所以这里不考虑互斥
    class ActionFactory final
    {
        public:
            static Action* create_action (const std::string& action_str)
            {
                if (actions_.size() == 0) { regist_all(); }
                
                Action* ptr = nullptr;
                
                for (auto itr = actions_.begin(); itr != actions_.end(); itr++)
                {
                    if (itr->first == action_str)
                    {
                        auto x = itr->second.get();
                        return itr->second.get();
                    }
                }
                
                EZLOG (jhtools::Log_level::ERR) << "no such action: " << action_str;
                return nullptr;
            }
            
            static void regist (std::shared_ptr<Action> action_ptr)
            {
                if (action_ptr.get() == nullptr || action_ptr->action_str().size() == 0)
                {
                    EZLOG (jhtools::Log_level::FATAL) << "can not regist this action: " \
                                                      << action_ptr->action_str() \
                                                      << " pointer: " << action_ptr.get();
                }
                
                else
                {
                    //有些key已经存在了就不能再用不然会发生歧义
                    if (actions_.find (action_ptr->action_str()) != actions_.end())
                    {
                        EZLOG (jhtools::Log_level::FATAL) << "resue key: " << action_ptr->action_str();
                    }
                    
                    else
                    { actions_.insert (std::make_pair (action_ptr->action_str(), action_ptr)); }
                }
            }
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