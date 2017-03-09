#include<iostream>
#include<string>
#include<algorithm>
#include"ezlog.h"
#include"directory.h"


using namespace std;

namespace jhtools
{
    
    cwStrDequePtr Directory::sub_dirs (bool refresh)
    {
        if (refresh) //rescan the dir
        {
            regular_files_ptr_.reset();
            sub_dir_ptr_.reset();
            scan_dir();
        }
        
        if (regular_files_ptr_ == nullptr && sub_dir_ptr_ == nullptr)
        {
            scan_dir();
        }
        
        return sub_dir_ptr_;
    }
    cwStrDequePtr Directory::regular_files (bool refresh)
    {
        if (refresh) //rescan the dir
        {
            regular_files_ptr_.reset();
            sub_dir_ptr_.reset();
            scan_dir();
        }
        
        if (regular_files_ptr_ == nullptr && sub_dir_ptr_ == nullptr)
        {
            scan_dir();
        }
        
        return regular_files_ptr_;
    }
    
    bool Directory::has_file (const std::wstring& file_name, bool refresh)
    {
        if (refresh || (sub_dir_ptr_ == nullptr && regular_files_ptr_ == nullptr))
        {
            scan_dir();
        }
        
        if (regular_files_ptr_->end() != find (regular_files_ptr_->begin(), regular_files_ptr_->end(), file_name))
        {
            return true;
        }
        
        else { return false; }
    }
    bool Directory::has_file (const std::string& file_name, bool refresh)
    {
        return has_file (string2wstring (file_name), refresh);
    }
    
    int Directory::count_all (bool refresh)
    {
        if (sub_dir_ptr_ == nullptr && regular_files_ptr_ == nullptr || refresh)
        {
            scan_dir();
        }
        
        return sub_dir_ptr_->size() + regular_files_ptr_->size();
    }
    int Directory::count_regular_file (bool refresh)
    {
        if (sub_dir_ptr_ == nullptr && regular_files_ptr_ == nullptr || refresh)
        {
            scan_dir();
        }
        
        return regular_files_ptr_->size();
    }
    
    //bool has_file(const std::string& file_name)const;
    //bool is_dir(const std::wstring& name, bool refresh = false);
    //bool is_regular_file(const std::wstring &name, bool refresh = false);
    
    void Directory::scan_dir()
    {
        sub_dir_ptr_.reset (new wStrDeque);
        regular_files_ptr_.reset (new wStrDeque);
        tinydir_dir dir;
        
        if (tinydir_open (&dir, path_.c_str()) == -1)
        {
            EZLOG (Log_level::FATAL) << "Can not open this dir :"<<wstring2string(path_);
        }
        
        while (dir.has_next)
        {
            tinydir_file file;
            tinydir_readfile (&dir, &file);
            
            //printf("%s", file.name);
            if (file.is_dir)
            {
                //printf("/");
                sub_dir_ptr_->push_back (file.name);
            }
            
            else
                if (file.is_reg)
                {
                    regular_files_ptr_->push_back (file.name);
                }
                
            //printf("\n");
            tinydir_next (&dir);
        }
        
        tinydir_close (&dir);
    }
}