#include<cstdio>
#include<ctime>
#include<functional>
#include"jhtools/ezlog.h"

using namespace std;

namespace jhtools
{
    /******************************* Instances for class Ezlog *************************************/
    //static value in ezlog
    EZlog* EZlog::log_instance_ = nullptr;
    string EZlog::log_file_;
    ofstream EZlog::log_file_stream_;
    mutex EZlog::log_file_mutex_;
    mutex EZlog::console_mutex_;
    const EZlog& avoid_race_conditions_for_EZlog_in_multithread_ (EZlog::Instance());
    //use EZlog::log(LEVEL) to choose different log level
    Logstream::~Logstream()
    {
        switch (level_)
        {
            case Log_level::INFO:
                EZlog::Instance().log_info (str() + suffix_);
                break;
                
            case Log_level::DBUG:
                EZlog::Instance().log_debug (str() + suffix_);
                break;
                
            case Log_level::WARN:
                EZlog::Instance().log_warn (str() + suffix_);
                break;
                
            case Log_level::ERR:
                EZlog::Instance().log_error (str() + suffix_);
                break;
                
            case Log_level::FATAL:
                EZlog::Instance().log_fatal (str() + suffix_);
                break;
        }
    }
    
}