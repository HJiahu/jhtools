/*
    Directory -- a simple class to explore directory in win and linux;
	just char string supported

    Copyright (c) 2017 HJiahu <HJiahu@outlook.com>

    All rights reserved. Use of this source code is governed by a
    MIT license that can be found in the LICENSE file.
*/

#ifndef JHTOOLS_DIRECTORY_H_
#define JHTOOLS_DIRECTORY_H_

#include<string>
#include<deque>
#include<memory>
#include"utils.h"	
#include"utils/tinyfiles.h"


namespace jhtools
{
    
    //all path has no suffix '/ '  eg. c:/pro/ggg
    class Directory
    {
        public:
			typedef std::deque<std::string >  StrDeque;
			typedef std::shared_ptr<StrDeque >  StrDequePtr;

            Directory (const std::string&path) : path_ (path) {}
            
            bool has_file (const std::string& file_name, bool refresh = false);
            //sub dirs with out iterator subs' subs
			StrDequePtr sub_dirs(bool refresh = false);
			//add suffix filter next time
			StrDequePtr regular_files(bool refresh = false);
			StrDequePtr regular_files(const std::string suffix, bool refresh = false);
			//if you do not have this function , regular_files("****") will call regular_files(bool refresh = true)
			StrDequePtr regular_files(const char *suffix, bool refresh = false);
			//cwStrDequePtr regular_files(bool refresh = false);
            int count_all (bool refresh = false); //dirs and regular files number in dir_
            int count_regular_file (bool refresh = false);
            
        protected:
            void scan_dir();
        private:
            std::string path_;
			StrDequePtr sub_dir_ptr_ = nullptr;
			StrDequePtr regular_files_ptr_ = nullptr;
    };
}

#endif // !JHTOOLS_DIRECTORY_H_
