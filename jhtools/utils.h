#ifndef JHTOOLS_UTILS_
#define JHTOOLS_UTILS_
#include<string>
#include<vector>
#include"ezlog.h"

namespace jhtools
{

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
    
    inline std::string replace_tail (const std::string& str, \
                                     const std::string& target_str, \
                                     const std::string& replace_str)
    {
        if (str.size() == 0 || target_str.size() == 0) { return str; }
        
        else
        {
            size_t pos = str.rfind (target_str);
            
            if (pos == std::string::npos || str.size() - pos != target_str.size()) { return str; }
            
            else
            {
                //std::string new_str(str);
                return std::string (str).replace (pos, str.size(), replace_str);
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
    
    inline bool startwith (const std::string &str, const std::string &tail)
    {
        if (str.size() == 0 || tail.size() == 0) { return false; }
        
        else
        {
            return str.compare (0, tail.size(), tail) == 0;
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
    
    inline bool remove (const std::string &file_path)
    {
        if (file_path.size() == 0)
        {
            EZLOG (Log_level::ERR) << "illegal file name.";
            return false;
        }
        
        if (0 != ::remove (file_path.c_str()))
        {
            EZLOG (Log_level::ERR) << "error when remove file :" << file_path;
            return false;
        }
        
        return true;
    }
    
	//return all substr between pre_delim and rear_delim in src_str 
    inline std::vector<std::string> substrs (
        const std::string &src_str,
        const std::string &pre_delim,const std::string &rear_delim)
    {
		std::vector<std::string> result;
		if (src_str.size() == 0)return result;
		else{
			int pos = 0;
			while (pos < src_str.size()){
				auto pre_delim_pos = src_str.find(pre_delim, pos);
				if (pre_delim_pos == std::string::npos)return result;
				auto rear_delim_pos = src_str.find(rear_delim, pos + pre_delim.size());
				if (rear_delim_pos == std::string::npos)return result;
				auto begin_pos = pre_delim_pos + pre_delim.size();
				result.push_back(src_str.substr(begin_pos, rear_delim_pos - begin_pos));
				pos = rear_delim_pos + rear_delim.size();
			}
			return result;
		}
    }
}

#endif // !JHTOOLS_UTILS_
