#include"jhtools/ezlog.h"

#include<cstdio>
#include<ctime>
#include<functional>
#include"jhtools/utils/termcolor.hpp"

using namespace std;

namespace jhtools
{
    //static value in ezlog
    EZlog* EZlog::log_instance_ = nullptr;
    string EZlog::log_file_;
    ofstream EZlog::log_file_stream_;
    mutex EZlog::log_file_mutex_;
    mutex EZlog::console_mutex_;
    
    EZlog::EZlog() //There is only one EZlog instance in the process (singleton)
    {
        log_file_ = LOG_FILE_NAME;
        log_file_stream_ = std::ofstream (log_file_, std::ios::app);
        
        if (log_file_stream_.rdstate() != std::ios_base::goodbit)
        {
            std::cout << termcolor::on_magenta << "can not create file :" << log_file_ << \
                      "\nPlease make sure you have appropriate permission!" << endl;
        }
    }
    
    shared_ptr<Logstream> EZlog::log (Log_level level, const std::string suffix)
    {
        return shared_ptr<Logstream> (new Logstream (level, suffix));
    }
    
    void EZlog::log_info (const std::string&msg)
    {
        string str (current_time_YMDT() + "[INFO]   " + msg);
        
        if (Log_level::INFO >= LOG_LEVEL)
        { writeline2logfile (str); }
        
        writeline2console (str, TERM_COLOR::ON_GREY);
    }
    void EZlog::log_warn (const std::string&msg)
    {
        string str (current_time_YMDT() + "[WARN]   " + msg);
        
        if (Log_level::WARN >= LOG_LEVEL)
        { writeline2logfile (str); }
        
        writeline2console (str, TERM_COLOR::YELLOW);
    }
    void EZlog::log_error (const std::string&msg)
    {
        string str (current_time_YMDT() + "[ERROR]  " + msg);
        
        if (Log_level::ERR >= LOG_LEVEL)
        { writeline2logfile (str); }
        
        writeline2console (str, TERM_COLOR::RED);
    }
    void EZlog::log_debug (const std::string&msg)
    {
        string str (current_time_YMDT() + "[DEBUG]  " + msg);
        
        if (Log_level::DBUG >= LOG_LEVEL)
        { writeline2logfile (str); }
        
        writeline2console (str, TERM_COLOR::CYAN);
    }
    void EZlog::log_fatal (const std::string&msg)
    {
        string str (current_time_YMDT() + "[FATAL]  " + msg);
        
        if (Log_level::FATAL >= LOG_LEVEL)
        { writeline2logfile (str); }
        
        writeline2console (str, TERM_COLOR::ON_RED);
        getchar();
        exit (1);
    }
    
    
    //help functions in EZlog
    string EZlog::current_time_YMDT()
    {
        char datetime[99];
        time_t current_t = time (nullptr);
        struct tm current_time;
        localtime_s (&current_time, &current_t);
        sprintf_s (datetime, \
                   "%d-%02d-%02d %02d:%02d:%02d", \
                   1900 + current_time.tm_year, \
                   1 + current_time.tm_mon, \
                   current_time.tm_mday, \
                   current_time.tm_hour, \
                   current_time.tm_min, \
                   current_time.tm_sec);
        return string (datetime) + " ";
    }
    inline void EZlog::writeline2logfile (const string& msg, const string &endwith)
    {
        log_file_mutex_.lock();
        
        if (log_file_stream_.rdstate() == ios_base::goodbit)
        { log_file_stream_ << msg << endwith << flush; }
        
        else
        { cout << termcolor::on_magenta << "can not write to log file:" << log_file_ << endl; };
        
        log_file_mutex_.unlock();
    }
    inline void EZlog::writeline2console (const std::string&msg, TERM_COLOR color, const std::string &endwith)
    {
#ifndef NO_TERMINAL_DISP
        console_mutex_.lock();
        string msg_ = string ("[Ezlog] ") + msg;
        
        switch (color)
        {
            case TERM_COLOR::GREY:
                cout << termcolor::grey << msg_ << endwith;
                break;
                
            case TERM_COLOR::RED:
                cout << termcolor::red << msg_ << endwith;
                break;
                
            case TERM_COLOR::GREEN:
                cout << termcolor::green << msg_ << endwith;
                break;
                
            case TERM_COLOR::YELLOW:
                cout << termcolor::yellow << msg_ << endwith;
                break;
                
            case TERM_COLOR::BLUE:
                cout << termcolor::blue << msg_ << endwith;
                break;
                
            case TERM_COLOR::MAGENTA:
                cout << termcolor::magenta << msg_ << endwith;
                break;
                
            case TERM_COLOR::CYAN:
                cout << termcolor::cyan << msg_ << endwith;
                break;
                
            case TERM_COLOR::WHITE:
                cout << termcolor::white << msg_ << endwith;
                break;
                
            case TERM_COLOR::ON_GREY:
                cout << termcolor::on_grey << msg_ << endwith;
                break;
                
            case TERM_COLOR::ON_RED:
                cout << termcolor::on_red << msg_ << endwith;
                break;
                
            case TERM_COLOR::ON_GREEN:
                cout << termcolor::on_green << msg_ << endwith;
                break;
                
            case TERM_COLOR::ON_YELLOW:
                cout << termcolor::on_yellow << msg_ << endwith;
                break;
                
            case TERM_COLOR::ON_BLUE:
                cout << termcolor::on_blue << msg_ << endwith;
                break;
                
            case TERM_COLOR::ON_MAGENTA:
                cout << termcolor::on_magenta << msg_ << endwith;
                break;
                
            case TERM_COLOR::ON_CYAN:
                cout << termcolor::on_cyan << msg_ << endwith;
                break;
                
            case TERM_COLOR::ON_WHITE:
                cout << termcolor::on_white << msg_ << endwith;
                break;
                
            default:
                cout << termcolor::underline << msg_ << endwith;
                break;
        }
        
        cout << termcolor::reset;
        console_mutex_.unlock();
#endif //NO_TERMINAL_DISP  
    }
    
	static const auto const avoid_race_conditions_for_EZlog_in_multithread_(EZlog::Instance());
}

