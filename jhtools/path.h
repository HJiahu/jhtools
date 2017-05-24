/*
    Path.h -- A simple class for manipulating paths on Linux/Windows/Mac OS

    Copyright (c) 2015 Wenzel Jakob <wenzel@inf.ethz.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#ifndef JHTOOLS_PATH_
#define JHTOOLS_PATH_

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <deque>
#include"utils/tinyfiles.h"

namespace jhtools
{
#if defined(_WIN32)
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <sys/stat.h>

#if defined(__linux)
    #include <linux/limits.h>
#endif
    /**
     * \brief Simple class for manipulating paths on Linux/Windows/Mac OS
     *
     * This class is just a temporary workaround to avoid the heavy boost
     * dependency until boost::filesystem is integrated into the standard template
     * library at some point in the future.
     */
    class Path;
    //return -1 if error
	//. and .. is stroed in name_deque, just like command ls 
    static int list_dir_use_tinyfile (const Path&path, std::deque<std::string> &name_deque, const std::string& suffix = "");
    
    class Path
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
            Path() : type_ (path_type::native_path), absolute_ (false) { }
            
            Path (const Path &Path)
                : type_ (Path.type_), path_ (Path.path_), absolute_ (Path.absolute_) {}
                
            Path (Path &&Path)
                : type_ (Path.type_), path_ (std::move (Path.path_)),
                  absolute_ (Path.absolute_) {}
                  
            Path (const char *string) { set (string); }
            
            Path (const std::string &string) { set (string); }
            
#if defined(_WIN32)
            Path (const std::wstring &wstring) { set (wstring); }
            Path (const wchar_t *wstring) { set (wstring); }
#endif
            
            size_t length() const { return path_.size(); }
            
            bool empty() const { return path_.empty(); }
            
            bool is_absolute() const { return absolute_; }
            
            Path make_absolute() const
            {
#if !defined(_WIN32)
                char temp[PATH_MAX];
                
                if (realpath (str().c_str(), temp) == NULL)
                { throw std::runtime_error ("Internal error in realpath(): " + std::string (strerror (errno))); }
                
                return Path (temp);
#else
                std::wstring value = wstr(), out (MAX_PATH, '\0');
                DWORD length = GetFullPathNameW (value.c_str(), MAX_PATH, &out[0], NULL);
                
                if (length == 0)
                { throw std::runtime_error ("Internal error in realpath(): " + std::to_string (GetLastError())); }
                
                return Path (out.substr (0, length));
#endif
            }
            
            bool exists() const
            {
#if defined(_WIN32)
                return GetFileAttributesW (wstr().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
                struct stat sb;
                return stat (str().c_str(), &sb) == 0;
#endif
            }
            
            size_t file_size() const
            {
#if defined(_WIN32)
                struct _stati64 sb;
                
                if (_wstati64 (wstr().c_str(), &sb) != 0)
                { throw std::runtime_error ("Path::file_size(): cannot stat file \"" + str() + "\"!"); }
                
#else
                struct stat sb;
                
                if (stat (str().c_str(), &sb) != 0)
                { throw std::runtime_error ("Path::file_size(): cannot stat file \"" + str() + "\"!"); }
                
#endif
                return (size_t) sb.st_size;
            }
            
            bool is_directory() const
            {
#if defined(_WIN32)
                DWORD result = GetFileAttributesW (wstr().c_str());
                
                if (result == INVALID_FILE_ATTRIBUTES)
                { return false; }
                
                return (result & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
                struct stat sb;
                
                if (stat (str().c_str(), &sb))
                { return false; }
                
                return S_ISDIR (sb.st_mode);
#endif
            }
            
            bool is_file() const
            {
#if defined(_WIN32)
                DWORD attr = GetFileAttributesW (wstr().c_str());
                return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
                struct stat sb;
                
                if (stat (str().c_str(), &sb))
                { return false; }
                
                return S_ISREG (sb.st_mode);
#endif
            }
            
            std::string extension() const
            {
                const std::string &name = filename();
                size_t pos = name.find_last_of (".");
                
                if (pos == std::string::npos)
                { return ""; }
                
                return name.substr (pos + 1);
            }
            
            std::string filename() const
            {
                if (empty())
                { return ""; }
                
                const std::string &last = path_[path_.size() - 1];
                return last;
            }
            
            Path parent_path() const
            {
                Path result;
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
            
            Path operator/ (const Path &other) const
            {
                if (other.absolute_)
                { throw std::runtime_error ("Path::operator/(): expected a relative Path!"); }
                
                if (type_ != other.type_)
                { throw std::runtime_error ("Path::operator/(): expected a Path of the same type!"); }
                
                Path result (*this);
                
                for (size_t i = 0; i < other.path_.size(); ++i)
                { result.path_.push_back (other.path_[i]); }
                
                return result;
            }
            
            std::string str (path_type type = path_type::native_path) const
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
            
            static int file_in_dir (const Path&path,
                                    std::deque<std::string> &names_deque,
									const std::string &suffix)
            {
				if (list_dir_use_tinyfile(path, names_deque, suffix) == -1)return -1;
				else{
					return names_deque.size();
				}
            }
            
            static bool list_dir (const Path&path, std::deque<std::string> &names_deque)
            {
                //file_in_dir has done: if (path.is_directory() && path.exists())
				return (list_dir_use_tinyfile(path, names_deque) == -1) ? false : true;
            }
            
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
            
            Path &operator= (const Path &Path)
            {
                type_ = Path.type_;
                path_ = Path.path_;
                absolute_ = Path.absolute_;
                return *this;
            }
            
            Path &operator= (Path &&Path)
            {
                if (this != &Path)
                {
                    type_ = Path.type_;
                    path_ = std::move (Path.path_);
                    absolute_ = Path.absolute_;
                }
                
                return *this;
            }
            
            friend std::ostream &operator<< (std::ostream &os, const Path &Path)
            {
                os << Path.str();
                return os;
            }
            
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
                { return false; }
                
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
            
            static Path getcwd()
            {
#if !defined(_WIN32)
                char temp[PATH_MAX];
                
                if (::getcwd (temp, PATH_MAX) == NULL)
                { throw std::runtime_error ("Internal error in getcwd(): " + std::string (strerror (errno))); }
                
                return Path (temp);
#else
                std::wstring temp (MAX_PATH, '\0');
                
                if (!_wgetcwd (&temp[0], MAX_PATH))
                { throw std::runtime_error ("Internal error in getcwd(): " + std::to_string (GetLastError())); }
                
                return Path (temp.c_str());
#endif
            }
            
#if defined(_WIN32)
            std::wstring wstr (path_type type = path_type::native_path) const
            {
                std::string temp = str (type);
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
            
            Path &operator= (const std::wstring &str) { set (str); return *this; }
#endif
            
            bool operator== (const Path &p) const { return p.path_ == path_; }
            bool operator!= (const Path &p) const { return p.path_ != path_; }
            
        protected:
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
    
    inline bool create_directory (const Path& p)
    {
#if defined(_WIN32)
        return CreateDirectoryW (p.wstr().c_str(), NULL) != 0;
#else
        return mkdir (p.str().c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
#endif
    }
    
	static int list_dir_use_tinyfile(const Path&path, std::deque<std::string> &name_deque, const std::string& suffix)
    {
        tfDIR dir;
        
        //this function will abort process when can not open dir
        if (!path.exists() && !path.is_directory()) { return -1; }
        
        tfDirOpen (&dir, path.str().c_str());
        
        while (dir.has_next)
        {
            tfFILE file;
            tfReadFile (&dir, &file);
            
            if (suffix.size() != 0 && std::string (file.ext) != suffix)
            {
                tfDirNext (&dir);
                continue;
            }
            
            else
            {
                name_deque.push_back (file.name);
            }
            
            tfDirNext (&dir);
        }
        
        tfDirClose (&dir);
    }
    
}//namespace jhtools

#endif // !JHTOOLS_PATH_