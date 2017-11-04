#ifndef CAFFE_TOLLS_ACTIONS_
#define CAFFE_TOLLS_ACTIONS_
#include<string>
#include<map>
#include<memory>
#include<algorithm>
#include"configs.h"

namespace caffe_tool_actions
{
    class Action
    {
        public:
            Action (const std::string& action_str, const jhtools::Json& config_json = configs_json_g)
                : action_str_ (action_str), config_json_ (config_json) {}
            const std::string& action_str() const { return action_str_; }
            virtual void execute() const = 0; //ִ�о������Ϊ
            void operator() () const
            {
                return execute();
            }
        protected:
            std::string action_str_;//������ǰ�������Ϊ
            const jhtools::Json& config_json_;
    };
    //ʹ�÷���ķ�ʽ����һ��action���󣬲����Ƕ��̵߳�ʹ���������ﲻ���ǻ���
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
                    //��Щkey�Ѿ������˾Ͳ������ò�Ȼ�ᷢ������
                    if (actions_.find (action_ptr->action_str()) != actions_.end())
                    {
                        EZLOG (jhtools::Log_level::FATAL) << "resue key: " << action_ptr->action_str();
                    }
                    
                    else
                    { actions_.insert (std::make_pair (action_ptr->action_str(), action_ptr)); }
                }
            }
        private:
            //ÿһ��д�ɵ�action���󶼽������������ע�ᵽaction����
            static void regist_all();
        private:
            static std::map < std::string, std::shared_ptr<Action> > actions_;
    };
    
    
    
    //�������Ϊ��ͼƬ���з��ಢ��ʾ������
    class ImgClassifier final : public Action
    {
        public:
            using Action::Action;
            void execute() const override;
    };
    //��һ���ļ����µ�ͼƬ���з��ಢ��ʾ������
    class DirImgsClassifier final : public Action
    {
        public:
            using Action::Action;
            void execute() const override;
    };
    
    //�������Ϊ���ڻ��ģ���ڵ�ǰ������ִ�е�ʱ��
    class ModelSpeed final: public Action
    {
        public:
            using Action::Action;
            void execute() const override;
    };
    
    
#endif // !CAFFE_TOLLS_ACTIONS_
    
    
}//caffe_tool_actions