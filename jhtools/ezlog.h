#ifndef EZLOG_h_
#define EZLOG_h_

#include<cstdio>
#include<ctime>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<mutex>
#include<memory>
#include"jhtools/termcolor.hpp"

namespace jhtools
{
	inline std::string current_time_YMDT();
    /****** configures ******/
    //logs will be storaged in file LOG_FILE_NAME
    static std::string LOG_FILE_NAME ("ezlog.log");
    
    //if defined this macro below ,all logs would not show on terminal.
    //#define NO_TERMINAL_DISP  //just storage msg in log file
    
    //level less than LOG_LEVEL will not write into log file
    enum class Log_level { INFO, DBUG, WARN, ERR, FATAL};
    const Log_level LOG_LEVEL = Log_level::INFO; //default all log msg will write into log file
    
    //if no define _DEBUG  macro, EZLOG_D (debug) will do nothing
    
    /****** end of configures ******/
    
    //use EZlog::log(LEVEL) to choose different log level
    class Logstream : public std::ostringstream
    {
        public:
            Logstream (Log_level level = Log_level::INFO, const std::string suffix = "") : level_ (level), suffix_ (suffix) {  }
            
            virtual ~Logstream();
            
        private:
            Log_level level_;
            std::string suffix_;//this string will be appended to log msg
    };
    
    template<typename T>
    std::shared_ptr<Logstream> operator<< (const std::shared_ptr<Logstream> &ptr, const T &data)
    {
        *ptr << data;
        return ptr; // ptr's life just in one scope
    }
    
    //Macro EZLOG depends on EZlog::log() and will append file and line number where EZLOG was called
#define EZLOG(level) \
	jhtools::EZlog::create_logstream(level,std::string(" [")+__FILE__+": "+std::to_string(__LINE__)+"]")
    
#ifdef _DEBUG
#define EZLOG_D(msg) \
	jhtools::EZlog::Instance().log_debug(msg+std::string(" [")+__FILE__+": "+std::to_string(__LINE__)+"]")
#else
#define EZLOG_D(msg)  ;
#endif //EZLOG_DEBUG_DO_NOTHING
    
    class EZlog
    {
        public:
            //EZlog() is private and defined below
            EZlog (const EZlog&) = default;
            //EZlog object can not move and copy
            EZlog& operator= (const EZlog&) = delete;
            EZlog (const EZlog&&) = delete;
            EZlog& operator= (const EZlog&&) = delete;
            
            //use this function to access methods of EZlog
            //to avoid race conditions in multithreads this function will be called
            //in ezlog.cpp after the defination of all funs in EZlog
            static EZlog& Instance()
            {
                if (!log_instance_)
                {
                    log_instance_ = new EZlog;
                }
                
                return *log_instance_;
                /* //use double if and mutex to avoid race conditons
                if(log_instance_ == nullptr){
                	mutex.lock();
                	if(log_instance_ == nullptr)
                		log_instance_ = new EZlog;
                	mutex.unlock();
                	else return log_instance_;
                }
                else return *log_instance_;
                
                */
            }
            
            void log_info (const std::string& msg) //white
            {
                std::string str (current_time_YMDT() + "[INFO]   " + msg);
                
                if (Log_level::INFO >= LOG_LEVEL)
                {
                    writeline2logfile (str);
                }
                
                writeline2console (str, TERM_COLOR::ON_GREY);
            }
            void log_debug (const std::string&msg) //cyan
            {
                std::string str (current_time_YMDT() + "[DEBUG]  " + msg);
                
                if (Log_level::DBUG >= LOG_LEVEL)
                {
                    writeline2logfile (str);
                }
                
                writeline2console (str, TERM_COLOR::CYAN);
            }
            void log_warn (const std::string& msg) //yellow
            {
                std::string str (current_time_YMDT() + "[WARN]   " + msg);
                
                if (Log_level::WARN >= LOG_LEVEL)
                {
                    writeline2logfile (str);
                }
                
                writeline2console (str, TERM_COLOR::YELLOW);
            }
            void log_error (const std::string&msg) //red
            {
                std::string str (current_time_YMDT() + "[ERROR]  " + msg);
                
                if (Log_level::ERR >= LOG_LEVEL)
                {
                    writeline2logfile (str);
                }
                
                writeline2console (str, TERM_COLOR::RED);
            }
            void log_fatal (const std::string&msg) //on_read
            {
                std::string str (current_time_YMDT() + "[FATAL]  " + msg);
                
                if (Log_level::FATAL >= LOG_LEVEL)
                {
                    writeline2logfile (str);
                }
                
                writeline2console (str, TERM_COLOR::ON_RED);
                getchar();
                exit (1);
            }
            
            static std::shared_ptr<Logstream> create_logstream (Log_level level = Log_level::INFO, const std::string suffix = "")
            {
                return std::shared_ptr<Logstream> (new Logstream (level, suffix));
            }
            
        private://hlep function
            void writeline2logfile (const std::string&msg, const std::string &endwith = "\n")
            {
                log_file_mutex_.lock();
                
                if (log_file_stream_.rdstate() == std::ios_base::goodbit)
                {
                    log_file_stream_ << msg << endwith << std::flush;
                }
                
                else
                {
                    std::cout << termcolor::on_magenta << "can not write to log file:" << log_file_ << std::endl;
                };
                
                log_file_mutex_.unlock();
            }
            enum class TERM_COLOR
            {
                GREY, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, ON_GREY, \
                ON_RED, ON_GREEN, ON_YELLOW, ON_BLUE, ON_MAGENTA, ON_CYAN, ON_WHITE
            };
            void writeline2console (const std::string&msg, TERM_COLOR color, const std::string &endwith = "\n")
            {
#ifndef NO_TERMINAL_DISP
                console_mutex_.lock();
                std::string msg_ = std::string ("[Ezlog] ") + msg;
                
                switch (color)
                {
                    case TERM_COLOR::GREY:
                        std::cout << termcolor::grey << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::RED:
                        std::cout << termcolor::red << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::GREEN:
                        std::cout << termcolor::green << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::YELLOW:
                        std::cout << termcolor::yellow << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::BLUE:
                        std::cout << termcolor::blue << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::MAGENTA:
                        std::cout << termcolor::magenta << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::CYAN:
                        std::cout << termcolor::cyan << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::WHITE:
                        std::cout << termcolor::white << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::ON_GREY:
                        std::cout << termcolor::on_grey << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::ON_RED:
                        std::cout << termcolor::on_red << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::ON_GREEN:
                        std::cout << termcolor::on_green << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::ON_YELLOW:
                        std::cout << termcolor::on_yellow << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::ON_BLUE:
                        std::cout << termcolor::on_blue << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::ON_MAGENTA:
                        std::cout << termcolor::on_magenta << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::ON_CYAN:
                        std::cout << termcolor::on_cyan << msg_ << endwith;
                        break;
                        
                    case TERM_COLOR::ON_WHITE:
                        std::cout << termcolor::on_white << msg_ << endwith;
                        break;
                        
                    default:
                        std::cout << termcolor::underline << msg_ << endwith;
                        break;
                }
                
                std::cout << termcolor::reset;
                console_mutex_.unlock();
#endif //NO_TERMINAL_DISP  
            }
            
        private:
            EZlog() //There is only one EZlog instance in the process (singleton)
            {
                //log_file_ and EZlog::log_file_stream_ is initialed in jhtools.cpp
                if (log_file_stream_.rdstate() != std::ios_base::goodbit)
                {
                    std::cout << termcolor::on_magenta << "can not create file :" << log_file_ << \
                              "\nPlease make sure you have appropriate permission!" << std::endl;
                }
            }
            
            
            static EZlog* log_instance_;
            static std::string log_file_;
            static std::ofstream log_file_stream_;
            static std::mutex log_file_mutex_;
            static std::mutex console_mutex_;
    };//class EZlog
    
    
    inline std::string current_time_YMDT()
    {
        char datetime[99];
        time_t current_t = time (nullptr);
        struct tm current_time;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s (&current_time, &current_t);
        sprintf_s (datetime, \
                   "%d-%02d-%02d %02d:%02d:%02d", \
                   1900 + current_time.tm_year, \
                   1 + current_time.tm_mon, \
                   current_time.tm_mday, \
                   current_time.tm_hour, \
                   current_time.tm_min, \
                   current_time.tm_sec);
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__)
        localtime_r (&current_t, &current_time);
        sprintf (datetime, \
                 "%d-%02d-%02d %02d:%02d:%02d", \
                 1900 + current_time.tm_year, \
                 1 + current_time.tm_mon, \
                 current_time.tm_mday, \
                 current_time.tm_hour, \
                 current_time.tm_min, \
                 current_time.tm_sec);
#endif
        return std::string (datetime) + " ";
    }
}

#endif //EZLOG_h_