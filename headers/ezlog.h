#ifndef EZLOG_h_
#define EZLOG_h_

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<mutex>
#include<memory>

namespace jhtools
{
    /****** configures ******/
    //logs will be storaged in file LOG_FILE_NAME
    static std::string LOG_FILE_NAME ("ezlog.log");
    
    //if defined this macro ,all logs would not show on terminal.
    //#define NO_TERMINAL_DISP  //just storage msg in log file
    
    //level less than LOG_LEVEL will not write into log file
    enum class Log_level { INFO, DBUG, WARN, ERR, FATAL};
    const Log_level LOG_LEVEL = Log_level::INFO; //default all log msg will write into log file
    
    /****** end of configures ******/
    
    class Logstream;
    
    
    //Macro EZLOG depends on EZlog::log() and will append file and line number where EZLOG was called
#define EZLOG(level) \
	jhtools::EZlog::log(level,std::string(" [")+__FILE__+": "+std::to_string(__LINE__)+"]")
    
    class EZlog
    {
        public:
            EZlog (const EZlog&) = delete;
            EZlog& operator= (const EZlog&) = delete;
			EZlog(const EZlog&&) = delete;
			EZlog& operator= (const EZlog&&) = delete;
            
            //use this function to access methods of EZlog
            static EZlog& Instance()
            {
                if (!log_instance_)
                {
                    log_instance_ = new EZlog;
                }
                
                return *log_instance_;
            }
            
            void log_info (const std::string&);//white
			void log_debug(const std::string&); //cyan
            void log_warn (const std::string&); //yellow
            void log_error (const std::string&); //red            
            void log_fatal (const std::string&); //on_read
            
            static std::shared_ptr<Logstream> log (Log_level = Log_level::INFO, const std::string suffix = "");
            
        private://hlep function
            std::string current_time_YMDT();
            void writeline2logfile (const std::string&, const std::string &endwith = "\n");
            enum class TERM_COLOR
            {
                GREY, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, ON_GREY, \
                ON_RED, ON_GREEN, ON_YELLOW, ON_BLUE, ON_MAGENTA, ON_CYAN, ON_WHITE
            };
            void writeline2console (const std::string&, TERM_COLOR color, const std::string &endwith = "\n");
            
        private:
			EZlog();//There is only one EZlog instance in the process (singleton)

            
            static EZlog* log_instance_;
            static std::string log_file_;
            static std::ofstream log_file_stream_;
            static std::mutex log_file_mutex_;
            static std::mutex console_mutex_;
    };
    
    
    //use EZlog::log(LEVEL) to choose different log level
    class Logstream : public std::ostringstream
    {
        public:
            Logstream (Log_level level = Log_level::INFO, \
                       const std::string suffix = "") : level_ (level), suffix_ (suffix) {  }
                       
            virtual ~Logstream()
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
            
        private:
            Log_level level_;
            std::string suffix_;//this string will be appended to log msg
    };
    
    template<typename T>
    std::shared_ptr<Logstream> operator<< (std::shared_ptr<Logstream> ptr, T data)
    {
        *ptr << data;
        return ptr; // ptr's life just in one scope
    }
    
}

#endif //EZLOG_h_