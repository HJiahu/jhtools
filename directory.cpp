/*
     -- 

    Copyright (c) 2017 HJiahu <HJiahu@outlook.com>

    All rights reserved. Use of this source code is governed by a
    MIT license that can be found in the LICENSE file.
*/
#include<iostream>
#include<string>
#include<algorithm>
#include"jhtools/ezlog.h"
#include"jhtools/directory.h"


using namespace std;

namespace jhtools
{
    
	Directory::StrDequePtr Directory::sub_dirs(bool refresh)
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
	Directory::StrDequePtr Directory::regular_files(bool refresh)
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
    
	Directory::StrDequePtr Directory::regular_files(const std::string suffix, bool refresh){
		shared_ptr<deque<string> > ptr(new deque<string>);
		for (const auto& x : *regular_files(refresh)){
			if (endwith(x, suffix))ptr->push_back(x);
		}

		return ptr;
	}
	//StrDequePtr regular_files(const std::string suffix, bool refresh = false);
	Directory::StrDequePtr Directory::regular_files(const char* suffix, bool refresh){
		shared_ptr<deque<string> > ptr(new deque<string>);
		for (const auto& x : *regular_files(refresh)){
			if (endwith(x, suffix))ptr->push_back(x);
		}

		return ptr;
	}
	bool Directory::has_file(const std::string& file_name, bool refresh)
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
		//
        sub_dir_ptr_.reset (new StrDeque);
        regular_files_ptr_.reset (new StrDeque);
		tfDIR dir;
        
		if (tfDirOpen(&dir, path_.c_str()) != 1)
        {
            EZLOG (Log_level::FATAL) << "Can not open this dir :"<<path_;
        }
        
        while (dir.has_next)
        {
			tfFILE file;
			tfReadFile(&dir, &file);
            
            //printf("%s", file.name);
            if (file.is_dir)
            {
                //printf("/");
                sub_dir_ptr_->push_back (file.name);
            }
            
            else
                if (file.is_reg)
                {
					regular_files_ptr_->push_back(file.name);
                }
                
            //printf("\n");
			tfDirNext(&dir);
        }
        
		tfDirClose(&dir);
    }
}