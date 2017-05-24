#ifndef JHTOOLS_XML_
#define JHTOOLS_XML_

#include<string>
#include<array>
#include<vector>
#include"utils/tinyxml2.h"
#include"ezlog.h"

namespace jhtools
{
    typedef std::array<int, 3> array3i;
    typedef std::array<int, 4> array4i;

    std::ostream& operator<< (std::ostream&, const array4i&);
    std::ostream& operator<< (std::ostream&, const std::vector<array4i >&);
    
    template<typename T>
    array3i make_array3i (T w, T h, T d)
    {
        array3i x;
        x[0] = static_cast<int> (w);
        x[1] = static_cast<int> (h);
        x[2] = static_cast<int> (d);
        return x;
    }
    template<typename T>
    array4i make_array4i (T x0, T y0, T x1, T y1)
    {
        array4i x;
        x[0] = static_cast<int> (x0);
        x[1] = static_cast<int> (y0);
        x[2] = static_cast<int> (x1);
        x[3] = static_cast<int> (y1);
        return x;
    }
    
    class Xml
    {
        public:
           // typedef std::multimap<std::string, std::string> XmlTextMap;
            Xml (const std::string&);
            
            void load_xml_file (const std::string &xml_file, bool refresh_xml_text_map = true);
            
            std::string file_name() const;
            std::string file_path() const;
            std::vector<array4i> bboxs() const;
            array3i img_size() const;
        private:
            std::string xml_file_path_;//with name
            tinyxml2::XMLDocument xml_doc_;
			//XmlTextMap xml_text_map_;
    };

	std::vector<array4i> bbxs_in_xml(const std::string&xml_file_path);
}

#endif // !JHTOOLS_XML_
