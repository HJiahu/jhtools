#ifndef JHTOOLS_DIRECTORY_H_
#define JHTOOLS_DIRECTORY_H_

#include<string>
#include<deque>
#include<memory>
#include"utils.h"	
#include"utils/tinydir.h"


namespace jhtools
{

    typedef std::deque<std::wstring> wStrDeque;
    typedef std::shared_ptr<wStrDeque> wStrDequePtr;
    typedef std::shared_ptr<const wStrDeque> cwStrDequePtr;
    
    //all path has no suffix '/ '  eg. c:/pro/ggg
    class Directory
    {
        public:
            Directory (const std::wstring& path) : path_ (path) {};
            Directory (const std::string&path) : path_ (string2wstring (path)) {}
            
            bool has_file (const std::wstring& file_name, bool refresh = false);
            bool has_file (const std::string& file_name, bool refresh = false);
            //sub dirs with out iterator subs' subs
            cwStrDequePtr sub_dirs (bool refresh = false);
            cwStrDequePtr regular_files (bool refresh = false);
            int count_all (bool refresh = false); //dirs and regular files number in dir_
            int count_regular_file (bool refresh = false);
            //static bool is_dir (const std::wstring& name, bool refresh = false);
            //static bool is_regular_file (const std::wstring &name, bool refresh = false);
            //static bool is_dir (const std::string& name, bool refresh = false);
            //static bool is_regular_file (const std::string &name, bool refresh = false);
            
        protected:
            void scan_dir();
        private:
            std::wstring path_;
            wStrDequePtr sub_dir_ptr_ = nullptr;
            wStrDequePtr regular_files_ptr_ = nullptr;
    };
}

#endif // !JHTOOLS_DIRECTORY_H_
