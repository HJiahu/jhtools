#ifndef JHTOOLS_UTILS_
#define JHTOOLS_UTILS_
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include"ezlog.h"
#include"path.h"
#include"json11.h"

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
    inline json11::Json stojson (const std::string& json_str);
    inline json11::Json ftojson (const std::string& file); //读取文件并将其中的内容转化为json对象
    
    
    
    
    
    
    
    
    
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
    
    inline json11::Json stojson (const std::string& json_str)
    {
        if (json_str.size() == 0) { return json11::Json(); }
        
        else
        {
            std::string err;
            auto j = json11::Json::parse (json_str, err);
            
            if (err.size() != 0)
            { EZLOG (jhtools::Log_level::FATAL) << "in stojson: " << err; }
            
            return j;
        }
    }//stojson
    
    inline json11::Json ftojson (const std::string& file)
    {
        if (!exists (path (file)))
        {
            EZLOG (Log_level::FATAL) << "can not open file :" << file;
            return json11::Json();
        }
        
        else
        {
            std::ifstream in (file);
            std::string str (static_cast<std::stringstream const&> (std::stringstream() << in.rdbuf()).str());
            return stojson (str);
        }
    }//ftojson
}//namespace jhtools



#endif // !JHTOOLS_UTILS_
