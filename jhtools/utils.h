#ifndef JHTOOLS_UTILS_
#define JHTOOLS_UTILS_
#include<string>
#include<vector>
#include"ezlog.h"

namespace jhtools
{
	
    //funs declarations
    inline std::wstring string2wstring (const std::string&str);
    inline std::string  wstring2string (const std::wstring&wstr);
    inline std::string current_time_YMDT();//2017-01-01 14:24:11
    inline bool replace_tail (std::string& str, const std::string& tail_str, const std::string& new_str);
    inline bool endwith (const std::string &str, const std::string &tail);
    inline bool startwith (const std::string &str, const std::string &leader_str);
    inline bool copy_file (const std::string &src_file, const std::string &dst_file);
    //return all substrs between pre_delim and rear_delim in src_str
    inline std::vector<std::string> substrs (const std::string &src_str, const std::string &pre_delim, const std::string &rear_delim);
    
    
    
    
    
    
    
    
    
    
    
    /******************************* inline or static implements******************************* */
    
    
    inline std::wstring string2wstring (const std::string&str)
    {
        std::wstring result;
        
        for (const auto &x : str)
        {
            result += static_cast<wchar_t> (x);
        }
        
        return result;
    }
    inline std::string  wstring2string (const std::wstring&wstr)
    {
        std::string result;
        
        for (const auto &x : wstr)
        {
            result += static_cast<char> (x);
        }
        
        return result;
    }
    
    inline std::string current_time_YMDT()
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
        return std::string (datetime);
    }
    
    // in place replace, return true if replace successfully (changed the str string )
    //retrun false if str was not been changed
    inline bool replace_tail (std::string& str, \
                              const std::string& tail_str, \
                              const std::string& new_str)
    {
        if (str.size() == 0 || tail_str.size() == 0) { return false; }
        
        else
        {
            size_t pos = str.rfind (tail_str);
            
            //str.size() - pos != tail_str.size() is used to check tail_str is in str but is not a tail string
            if (pos == std::string::npos || str.size() - pos != tail_str.size()) { return false; }
            
            else
            {
                //std::string new_str(str);
                str.replace (pos, str.size(), new_str);
                return true;
            }
        }
    }
    
    inline bool endwith (const std::string &str, const std::string &tail)
    {
        if (str.size() == 0 || tail.size() == 0) { return false; }
        
        else
        {
            return str.compare (str.size() - tail.size(), tail.size(), tail) == 0;
        }
    }
    
    inline bool startwith (const std::string &str, const std::string &leader_str)
    {
        if (str.size() == 0 || leader_str.size() == 0) { return false; }
        
        else
        {
            return str.compare (0, leader_str.size(), leader_str) == 0;
        }
    }
    
    inline bool copy_file (const std::string &src_file, const std::string &dst_file)
    {
        if (src_file.size() == 0 || dst_file.size() == 0)
        {
            EZLOG (Log_level::ERR) << "file path error.";
            return false;
        }
        
        std::ifstream src (src_file, std::ios::binary);
        
        if (src.rdstate() != std::ios_base::goodbit)
        {
            EZLOG (Log_level::ERR) << "Can not read file :" << src_file;
            return false;
        }
        
        std::ofstream dst (dst_file, std::ios::binary);
        
        if (dst.rdstate() != std::ios_base::goodbit)
        {
            EZLOG (Log_level::ERR) << "Can not create file :" << dst_file;
            return false;
        }
        
        dst << src.rdbuf();
        src.close();
        dst.close();
        return true;
    }
    
    inline std::vector<std::string> substrs (
        const std::string &src_str,
        const std::string &pre_delim,
        const std::string &rear_delim)
    {
        std::vector<std::string> result;
        
        if (src_str.size() == 0) { return result; }
        
        else
        {
            size_t pos = 0;
            
            while (pos < src_str.size())
            {
                auto pre_delim_pos = src_str.find (pre_delim, pos);
                
                if (pre_delim_pos == std::string::npos) { return result; }
                
                auto rear_delim_pos = src_str.find (rear_delim, pos + pre_delim.size());
                
                if (rear_delim_pos == std::string::npos) { return result; }
                
                auto begin_pos = pre_delim_pos + pre_delim.size();
                result.push_back (src_str.substr (begin_pos, rear_delim_pos - begin_pos));
                pos = rear_delim_pos + rear_delim.size();
            }
            
            return result;
        }
    }
}

#endif // !JHTOOLS_UTILS_
