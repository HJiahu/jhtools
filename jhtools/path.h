/*
    path.h -- A simple class for manipulating paths on Linux/Windows/Mac OS

    Copyright (c) 2015 Wenzel Jakob <wenzel@inf.ethz.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#ifndef JHTOOLS_PATH_
#define JHTOOLS_PATH_

//use boost.filesystem or C++17 filesystem if define one of these MACRO
//#define USE_BOOST_FILESYSTEM_IN_JHTOOLS
//#define USE_CPPVER17_FILESYSTEM_IN_JHTOOLS

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <deque>
#if !defined(USE_BOOST_FILESYSTEM_IN_JHTOOLS) && !defined(USE_CPPVER17_FILESYSTEM_IN_JHTOOLS)
    //#include "utils/tinyfiles.h"
#elif defined(USE_BOOST_FILESYSTEM_IN_JHTOOLS)
    #include<boost/filesystem.hpp>
#elif defined(USE_CPPVER17_FILESYSTEM_IN_JHTOOLS)
    #include<USE_CPPVER17_FILESYSTEM_IN_JHTOOLS>
#endif // !USE_FILESYSTEM_IN_JHTOOLS



#if  !defined(USE_BOOST_FILESYSTEM_IN_JHTOOLS) && !defined(USE_CPPVER17_FILESYSTEM_IN_JHTOOLS)

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <sys/stat.h>

#if defined(__linux)
    #include <linux/limits.h>
#endif
namespace jhtools
{
    /**
     * \brief Simple class for manipulating paths on Linux/Windows/Mac OS
     *
     * This class is just a temporary workaround to avoid the heavy boost
     * dependency until boost::filesystem is integrated into the standard template
     * library at some point in the future.
     */
    class path;
    //return -1 if error
    //. and .. is stroed in name_deque, just like command ls
    inline path current_path();
    inline bool create_directory (const path& p);
    inline bool is_directory (const path&p);
    inline bool is_regular_file (const path&p);
    inline size_t file_size (const path&p);
    inline bool exists (const path&p);
    class path
    {
        public:
            enum class path_type
            {
                windows_path = 0,
                posix_path = 1,
#if defined(_WIN32)
                native_path = windows_path
#else
                native_path = posix_path
#endif
            };
            //path_type type_; 	std::vector<std::string> path_;	bool absolute_;
            path() : type_ (path_type::native_path), absolute_ (false) { }
            
            path (const path &path)
                : type_ (path.type_), path_ (path.path_), absolute_ (path.absolute_) {}
                
            path (path &&path)
                : type_ (path.type_), path_ (std::move (path.path_)),
                  absolute_ (path.absolute_) {}
                  
            path (const char *string) { set (string); }
            
            path (const std::string &string) { set (string); }
            
#if defined(_WIN32)
            path (const std::wstring &wstring) { set (wstring); }
            path (const wchar_t *wstring) { set (wstring); }
#endif
            
            bool empty() const { return path_.empty(); }
            
            bool is_absolute() const { return absolute_; }
            
            
            std::string extension() const
            {
                const std::string &name = filename();
                size_t pos = name.find_last_of (".");
                
                if (pos == std::string::npos)
                { return ""; }
                
                return name.substr (pos);
            }
            
            std::string filename() const
            {
                if (empty())
                { return ""; }
                
                const std::string &last = path_[path_.size() - 1];
                return last;
            }
            
            path parent_path() const
            {
                path result;
                result.absolute_ = absolute_;
                
                if (path_.empty())
                {
                    if (!absolute_)
                    { result.path_.push_back (".."); }
                }
                
                else
                {
                    size_t until = path_.size() - 1;
                    
                    for (size_t i = 0; i < until; ++i)
                    { result.path_.push_back (path_[i]); }
                }
                
                return result;
            }
            
            path operator/ (const path &other) const
            {
                if (other.absolute_)
                { throw std::runtime_error ("path::operator/(): expected a relative path!"); }
                
                if (type_ != other.type_)
                { throw std::runtime_error ("path::operator/(): expected a path of the same type!"); }
                
                path result (*this);
                
                for (size_t i = 0; i < other.path_.size(); ++i)
                { result.path_.push_back (other.path_[i]); }
                
                return result;
            }
            
            std::string string (path_type type = path_type::native_path) const
            {
                std::ostringstream oss;
                
                if (type_ == path_type::posix_path && absolute_)
                { oss << "/"; }
                
                for (size_t i = 0; i < path_.size(); ++i)
                {
                    oss << path_[i];
                    
                    if (i + 1 < path_.size())
                    {
                        if (type == path_type::posix_path)
                        { oss << '/'; }
                        
                        else
                        { oss << '\\'; }
                    }
                }
                
                return oss.str();
            }
            
            path &operator= (const path &path)
            {
                type_ = path.type_;
                path_ = path.path_;
                absolute_ = path.absolute_;
                return *this;
            }
            
            path &operator= (path &&path)
            {
                if (this != &path)
                {
                    type_ = path.type_;
                    path_ = std::move (path.path_);
                    absolute_ = path.absolute_;
                }
                
                return *this;
            }
            
            friend std::ostream &operator<< (std::ostream &os, const path &path)
            {
                os << path.string();
                return os;
            }
            
            bool operator== (const path &p) const { return p.path_ == path_; }
            bool operator!= (const path &p) const { return p.path_ != path_; }
            
        protected:
            friend path current_path();
            friend bool is_directory (const path&p);
            friend bool create_directory (const path& p);
            friend bool is_regular_file (const path&p);
            friend size_t file_size (const path&p);
            friend bool exists (const path&p);
            
            
            bool remove_file()
            {
#if !defined(_WIN32)
                return std::remove (str().c_str()) == 0;
#else
                return DeleteFileW (wstr().c_str()) != 0;
#endif
            }
            
            bool resize_file (size_t target_length)
            {
#if !defined(_WIN32)
                return ::truncate (str().c_str(), (off_t) target_length) == 0;
#else
                HANDLE handle = CreateFileW (wstr().c_str(), GENERIC_WRITE, 0, nullptr, 0, FILE_ATTRIBUTE_NORMAL, nullptr);
                
                if (handle == INVALID_HANDLE_VALUE)
                {
                    return false;
                }
                
                LARGE_INTEGER size;
                size.QuadPart = (LONGLONG) target_length;
                
                if (SetFilePointerEx (handle, size, NULL, FILE_BEGIN) == 0)
                {
                    CloseHandle (handle);
                    return false;
                }
                
                if (SetEndOfFile (handle) == 0)
                {
                    CloseHandle (handle);
                    return false;
                }
                
                CloseHandle (handle);
                return true;
#endif
            }
            
            static path getcwd()
            {
#if !defined(_WIN32)
                char temp[PATH_MAX];
                
                if (::getcwd (temp, PATH_MAX) == NULL)
                {
                    throw std::runtime_error ("Internal error in getcwd(): " + std::string (strerror (errno)));
                }
                
                return path (temp);
#else
                std::wstring temp (MAX_PATH, '\0');
                
                if (!_wgetcwd (&temp[0], MAX_PATH))
                {
                    throw std::runtime_error ("Internal error in getcwd(): " + std::to_string (GetLastError()));
                }
                
                return path (temp.c_str());
#endif
            }
            
            
#if defined(_WIN32)
            std::wstring wstr (path_type type = path_type::native_path) const
            {
                std::string temp = string (type);
                int size = MultiByteToWideChar (CP_UTF8, 0, &temp[0], (int) temp.size(), NULL, 0);
                std::wstring result (size, 0);
                MultiByteToWideChar (CP_UTF8, 0, &temp[0], (int) temp.size(), &result[0], size);
                return result;
            }
            
            void set (const std::wstring &wstring, path_type type = path_type::native_path)
            {
                std::string string;
                
                if (!wstring.empty())
                {
                    int size = WideCharToMultiByte (CP_UTF8, 0, &wstring[0], (int) wstring.size(),
                                                    NULL, 0, NULL, NULL);
                    string.resize (size, 0);
                    WideCharToMultiByte (CP_UTF8, 0, &wstring[0], (int) wstring.size(),
                                         &string[0], size, NULL, NULL);
                }
                
                set (string, type);
            }
            
            path &operator= (const std::wstring &str) { set (str); return *this; }
#endif
            void set (const std::string &str, path_type type = path_type::native_path)
            {
                type_ = type;
                
                if (type == path_type::windows_path)
                {
                    path_ = tokenize (str, "/\\");
                    //C:/...
                    absolute_ = str.size() >= 2 && std::isalpha (str[0]) && str[1] == ':';
                }
                
                else
                {
                    path_ = tokenize (str, "/");
                    absolute_ = !str.empty() && str[0] == '/';
                }
            }
            
            bool is_directory() const //filesystem::path does not has member function is_directory()
            {
#if defined(_WIN32)
                DWORD result = GetFileAttributesW (wstr().c_str());
                
                if (result == INVALID_FILE_ATTRIBUTES)
                {
                    return false;
                }
                
                return (result & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
                struct stat sb;
                
                if (stat (str().c_str(), &sb))
                {
                    return false;
                }
                
                return S_ISDIR (sb.st_mode);
#endif
            }
            
            bool is_file() const //filesystem::path does not has member function is_file
            {
#if defined(_WIN32)
                DWORD attr = GetFileAttributesW (wstr().c_str());
                return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
                struct stat sb;
                
                if (stat (str().c_str(), &sb))
                {
                    return false;
                }
                
                return S_ISREG (sb.st_mode);
#endif
            }
            
            size_t file_size() const ////filesystem::path does not has member function exists
            {
#if defined(_WIN32)
                struct _stati64 sb;
                
                if (_wstati64 (wstr().c_str(), &sb) != 0)
                {
                    throw std::runtime_error ("path::file_size(): cannot stat file \"" + string() + "\"!");
                }
                
#else
                struct stat sb;
                
                if (stat (str().c_str(), &sb) != 0)
                {
                    throw std::runtime_error ("path::file_size(): cannot stat file \"" + str() + "\"!");
                }
                
#endif
                return (size_t) sb.st_size;
            }
            
            
            bool exists() const  //filesystem::path does not has member function exists
            {
#if defined(_WIN32)
                return GetFileAttributesW (wstr().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
                struct stat sb;
                return stat (str().c_str(), &sb) == 0;
#endif
            }
            
            static std::vector<std::string> tokenize (const std::string &string, const std::string &delim)
            {
                std::string::size_type lastPos = 0, pos = string.find_first_of (delim, lastPos);
                std::vector<std::string> tokens;
                
                while (lastPos != std::string::npos)
                {
                    if (pos != lastPos)
                    { tokens.push_back (string.substr (lastPos, pos - lastPos)); }
                    
                    lastPos = pos;
                    
                    if (lastPos == std::string::npos || lastPos + 1 == string.length())
                    { break; }
                    
                    pos = string.find_first_of (delim, ++lastPos);
                }
                
                return tokens;
            }
            
        protected:
            path_type type_;
            std::vector<std::string> path_;
            bool absolute_;
    };
    
    //*******************   assist functions for path   ************************
    
    inline path current_path()
    {
        return path::getcwd();
    }
    
    inline bool create_directory (const path& p)
    {
#if defined(_WIN32)
        return CreateDirectoryW (p.wstr().c_str(), NULL) != 0;
#else
        return mkdir (p.str().c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
#endif
    }
    
    inline bool is_directory (const path&p)
    {
        return p.is_directory();
    }
    
    inline bool is_regular_file (const path&p)
    {
        return p.is_file();
    }
    
    inline size_t file_size (const path&p)
    {
        return p.file_size();
    }
    
    inline bool exists (const path&p)
    {
        return p.exists();
    }
    
    
#else
    
#if defined(USE_BOOST_FILESYSTEM_IN_JHTOOLS) //using name path
    using boost::filesystem::path;
    using boost::filesystem::current_path;
    using boost::filesystem::create_directory;
    using boost::filesystem::is_directory;
    using boost::filesystem::is_regular_file;
    using boost::filesystem::file_size;
    using boost::filesystem::exists;
    namespace fs = boost::filesystem;
#elif defined(USE_CPPVER17_FILESYSTEM_IN_JHTOOLS)
    using std::filesystem::path;
    using std::filesystem::current_path;
    using std::filesystem::create_directory;
    using std::filesystem::is_directory;
    using std::filesystem::is_regular_file;
    using std::filesystem::file_size;
    using std::filesystem::exists;
    namespace fs = std::filesystem;
#endif //:~using name path
    
    
static int file_in_dir (const path&p,
                        std::deque<std::string> &names_deque,
                        const std::string &suffix = "")
{
    names_deque.clear();
    
    if (p.empty()) { return 0; }
    
    else
    {
        int file_count = 0;
    
        if (fs::exists (p))
        {
            if (fs::is_regular_file (p))
            {
                return -1;
            }
    
            else
                if (fs::is_directory (p))
                {
                    for (fs::directory_entry& x : fs::directory_iterator (p))
                    {
                        fs::path p_buff (x);
    
                        if (fs::is_regular_file (p_buff))
                        {
                            if (suffix.size() == 0)
                            {
                                file_count++;
                                names_deque.push_back (p_buff.filename().string());
                            }
    
                            else
                            {
                                if (p_buff.extension().string() == suffix)
                                {
                                    file_count++;
                                    names_deque.push_back (p_buff.filename().string());
                                }
                            }
                        }
                    }
                }
        }
    
        return file_count;
    }
}
    
static bool list_dir (const path &p, std::deque<std::string> &names_deque)
{
    if (p.empty()) { return false; }
    
    else
    {
        if (fs::exists (p))
        {
            for (fs::directory_entry& x : fs::directory_iterator (p))
            {
                names_deque.push_back (x.path().string());
            }
    
            return true;
        }
    
        else
        {
            return false;
        }
    }
}
    
#endif // !USE_FILESYSTEM_IN_JHTOOLS
}//namespace jhtools

#endif // !JHTOOLS_PATH_