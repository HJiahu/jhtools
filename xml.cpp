#include"xml.h"
#include"ezlog.h"
using namespace std;

namespace jhtools
{

    Xml::Xml (const string&xml_file) : xml_file_path_ (xml_file)
    {
        //if open file sucessfuly ,LoadFile will return 0
        auto failed = xml_doc_.LoadFile (xml_file_path_.c_str());        
        if (failed)
        {
			EZLOG(Log_level::FATAL) << "error in opening file: " << xml_file_path_;
        }
        
        //xml_file_name_ =
    }
    
	void Xml::load_xml_file(const string& file){
		xml_file_path_ = file;
		//if open file sucessfuly ,LoadFile will return 0
		auto failed = xml_doc_.LoadFile(xml_file_path_.c_str());
		if (failed)
		{
			EZLOG(Log_level::FATAL) << "error in opening file: " << xml_file_path_;
		}
	}

    string Xml::file_name()const{
		auto pos = xml_file_path_.find_last_of("\\/");
		if (pos == string::npos)return xml_file_path_;
		else{
			return string(xml_file_path_.begin() + pos + 1, xml_file_path_.end());
		}

    }
    
    string Xml::file_path() const
    {
        return xml_file_path_;
    }
    
    vector<array4i> Xml::bboxs() const
    {
        vector<array4i> bbs; //bunding boxs
        const tinyxml2::XMLElement* root = xml_doc_.RootElement();
        const tinyxml2::XMLElement* object = root->FirstChildElement ("object");
        
        while (object)
        {
            const tinyxml2::XMLElement * bndbox = object->FirstChildElement ("bndbox");
            
            if (bndbox == nullptr) { break; }
            
			const tinyxml2::XMLElement * bndbox_child_xmin = bndbox->FirstChildElement("xmin");
			const tinyxml2::XMLElement * bndbox_child_xmax = bndbox->FirstChildElement("xmax");
			const tinyxml2::XMLElement * bndbox_child_ymin = bndbox->FirstChildElement("ymin");
			const tinyxml2::XMLElement * bndbox_child_ymax = bndbox->FirstChildElement("ymax");
            
            if (bndbox_child_xmax == nullptr || bndbox_child_xmin == nullptr \
                    || bndbox_child_ymax == nullptr || bndbox_child_ymin == nullptr)
            {
                EZLOG (Log_level::ERR) << "bbox info error: " << file_name();
                object = object->NextSiblingElement();                
                if (object == nullptr || std::string (object->Name()) != "object") { break; }                
                continue;
            }


            int x_min = atoi (bndbox_child_xmin->GetText());
            int y_min = atoi (bndbox_child_ymin->GetText());
            int x_max = atoi (bndbox_child_xmax->GetText());
            int y_max = atoi (bndbox_child_ymax->GetText());

			array4i pos;
			pos[0] = x_min;
			pos[1] = y_min;
			pos[2] = x_max;
			pos[3] = y_max;

            int width = x_max - x_min;
            int height = y_max - y_min;
            bbs.push_back (pos);
            object = object->NextSiblingElement();
            
            if (object == nullptr || std::string (object->Name()) != "object") { break; }
        }
        
        return bbs;
    }
}