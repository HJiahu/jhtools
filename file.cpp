#include<fstream>
#include"ezlog.h"
#include"utils.h"
#include"file.h"

using namespace std;

namespace jhtools
{
    bool File::copy (const std::wstring &src_file, const std::wstring &dst_file)
    {
        if (src_file.size() == 0 || dst_file.size() == 0)
        {
            EZLOG (Log_level::ERR) << "file path error.";
            return false;
        }
        
        ifstream src (src_file, ios::binary);        
        if (src.rdstate() != ios_base::goodbit)
        {
            EZLOG (Log_level::ERR) << "Can not read file :" << wstring2string (src_file);
            return false;
        }
        
		ofstream dst(dst_file, ios::binary);
        if (dst.rdstate() != ios_base::goodbit)
        {
            EZLOG (Log_level::ERR) << "Can not create file :" << wstring2string (dst_file);
            return false;
        }
        
        dst << src.rdbuf();
        return true;
    }
    
    //bool File::copy(const std::string &src_file, const std::string &dst_file){
    
    //}
}