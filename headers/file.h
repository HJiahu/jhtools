#ifndef  JHTOOLS_FILE_
#define  JHTOOLS_FILE_
#include<string>

namespace jhtools{
	class File{
	public:
		static bool copy(const std::wstring &src_file, const std::wstring &dst_file);
		static bool copy(const std::string &src_file, const std::string &dst_file);
		static bool copy2dir(const std::wstring &src_file, const std::wstring &dst_dir);
		static bool copy2dir(const std::string &src_file, const std::string &dst_dir);
	};
}

#endif // ! JHTOOLS_FILE_
