#ifndef JHTOOLS_PATH_H_
#define JHTOOLS_PATH_H_
#include<string>
#include<vector>
#include<dirent.h>
#include"utils/path.h"
#include"ezlog.h"


namespace jhtools
{
    using TinyPath::path;
    
    inline path current_path();
    inline bool create_directory (const path& p);
    inline bool is_directory (const path&p);
    inline bool is_regular_file (const path&p);
    inline size_t file_size (const path&p);
    inline bool exists (const path&p);
    enum class ListFileType { ALL, FILE, DIR };//type for list in list_dir
    static std::vector<std::string> list_dir (const path& p, ListFileType type = ListFileType::FILE);
    
    
    
    
    /****************    implementtion     ****************/
    inline path current_path()
    {
        return path::getcwd();
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
    inline bool create_directory (const path& p)
    {
#if defined(_WIN32)
        return CreateDirectoryW (p.wstr().c_str(), NULL) != 0;
#else
        return mkdir (p.str().c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
#endif
    }
    //list directory
    //for directory do not return . and ..
#if defined(_WIN32)
    //for directory do not return . and ..
#include <windows.h>
    static std::vector<std::string> list_dir (const path& p, ListFileType type)
    {
        if (!exists (p))
        {
            EZLOG (Log_level::ERR) << "no file (dircetoy): " << p.string();
            return std::vector<std::string>();
        }
        
        WIN32_FIND_DATA ffd;
        LARGE_INTEGER filesize;
        HANDLE hFind = INVALID_HANDLE_VALUE;
        TCHAR szDir[MAX_PATH];
        int i = 0;
        std::string path_str (p.string());
        
        for (; i < path_str.size(); i++)
        {
            szDir[i] = TCHAR (path_str[i]);
        }
        
        szDir[i++] = (TCHAR) '/';
        szDir[i++] = (TCHAR) '*';
        szDir[i++] = (TCHAR) 0;
        std::vector<std::string> result;
        hFind = FindFirstFile (szDir, &ffd);
        auto list_type = FILE_ATTRIBUTE_ARCHIVE;
        
        switch (type)
        {
            case ListFileType::ALL:
                list_type = FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_DIRECTORY;
                break;
                
            case ListFileType::FILE:
                list_type = FILE_ATTRIBUTE_ARCHIVE;
                break;
                
            case ListFileType::DIR:
                list_type = FILE_ATTRIBUTE_DIRECTORY;
                break;
                
            default:
                break;
        }
        
        do
        {
            //FILE_ATTRIBUTE_ARCHIVE
            //FILE_ATTRIBUTE_DIRECTORY
            if (list_type & ffd.dwFileAttributes)
            {
                std::string str;
                
                for (const auto &x : ffd.cFileName)
                {
                    str += static_cast<char> (x);
                }
                
                result.push_back (str);
            }
            
            else
            {
                filesize.LowPart = ffd.nFileSizeLow;
                filesize.HighPart = ffd.nFileSizeHigh;
            }
        }
        while (FindNextFile (hFind, &ffd) != 0);
        
        FindClose (hFind);
        
        if (type != ListFileType::FILE && result.size() >= 2)
        {
            return std::vector<std::string> (result.begin() + 2, result.end());
        }
        
        else
        { return result; }
    }
#else
    //for directory do not return . and ..
    inline std::vector<std::string> list_dir (const std::string &dir_path, ListFileType type)
    {
        std::vector<std::string> files;
        auto root_path = path (dir_path);
    
        if (!exists (root_path))
        {
            EZLOG (Log_level::ERR) << "no such file or path: " << dir_path;
            return files;
        }
    
        DIR *dp = opendir (dir_path.c_str());
        struct dirent *dirp;
    
        while ( (dirp = readdir (dp)) != NULL)
        {
            if (is_directory (root_path / path (std::string (dirp->d_name))))
            {
                if (type != ListFileType::DIR) { files.push_back (std::string (dirp->d_name)); }
            }
    
            else
            {
                if (type != ListFileType::FILE) { files.push_back (std::string (dirp->d_name)); }
            }
        }
    
        closedir (dp);
    
        if (type != ListFileType::FILE && files.size() >= 2) { return std::vector<std::string> (files.begin() + 2, files.end()); }
    
        return files;
    }
#endif
}

#endif // !JHTOOLS_PATH_H_
