#ifndef JHTOOLS_XML_
#define JHTOOLS_XML_

#include<string>
#include<array>
#include<vector>
#include"utils/tinyxml2.h"
#include"ezlog.h"

namespace jhtools
{

    typedef std::array<int, 4> array4i;
    template<typename T>
    array4i make_array4i(T x0, T y0, T x1, T y1)
    {
		array4i x;
		x[0] = static_cast<int>(x0);
		x[1] = static_cast<int>(y0);
		x[2] = static_cast<int>(x1);
		x[3] = static_cast<int>(y1);

		return x;
    }
    
    
    class Xml
    {
        public:
            Xml (const std::string&);
            
            void load_xml_file (const std::string &xml_file);
            
            std::string file_name() const;
            std::string file_path() const;
            std::vector<array4i> bboxs() const;
        private:
            std::string xml_file_path_;//with name
            tinyxml2::XMLDocument xml_doc_;
    };
}

#endif // !JHTOOLS_XML_
