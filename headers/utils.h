#ifndef JHTOOLS_UTILS_
#define JHTOOLS_UTILS_
#include<string>

namespace jhtools{
	
	inline std::wstring string2wstring(const std::string&str)
	{
		std::wstring result;

		for (const auto &x : str)
		{
			result += x;
		}

		return result;
	}
	inline std::string  wstring2string(const std::wstring&wstr)
	{
		std::string result;

		for (const auto &x : wstr)
		{
			result += x;
		}

		return result;
	}

}

#endif // !JHTOOLS_UTILS_
