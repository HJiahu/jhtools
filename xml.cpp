#include"jhtools/xml.h"
#include"jhtools/ezlog.h"
#include"jhtools/path.h"
#include"jhtools/utils.h"
using namespace std;

namespace jhtools
{
    std::ostream& operator<< (std::ostream& cout_stream, const array4i& rhs)
    {
        cout_stream << "[(" << rhs[0] << ',' << rhs[2] << "), (" << rhs[2] << ',' << rhs[3] << ")]";
        return cout_stream;
    }
    
    std::ostream& operator<< (std::ostream& cout_stream, const vector<array4i>&vec_arr4i)
    {
        cout_stream << '{';
        
        for (const auto &x : vec_arr4i)
        {
            cout_stream << x << ", ";
        }
        
        cout_stream << '}';
        return cout_stream;
    }
    
    Xml::Xml (const string&xml_file) : xml_file_path_ (xml_file)
    {
        //if open file sucessfuly ,LoadFile will return 0
        auto failed = xml_doc_.LoadFile (xml_file_path_.c_str());
        
        if (failed)
        {
            EZLOG (Log_level::FATAL) << "error in opening file: " << xml_file_path_;
        }
        
        //xml_file_name_ =
    }
    
    void Xml::load_xml_file (const string& file, bool refresh_xml_text_map)
    {
        xml_file_path_ = file;
        //if open file sucessfuly ,LoadFile will return 0
        auto failed = xml_doc_.LoadFile (xml_file_path_.c_str());
        
        if (failed)
        {
            EZLOG (Log_level::FATAL) << "error in opening file: " << xml_file_path_;
        }
    }
    
    string Xml::file_name() const
    {
        auto pos = xml_file_path_.find_last_of ("\\/");
        
        if (pos == string::npos) { return xml_file_path_; }
        
        else
        {
            return string (xml_file_path_.begin() + pos + 1, xml_file_path_.end());
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
            
            const tinyxml2::XMLElement * bndbox_child_xmin = bndbox->FirstChildElement ("xmin");
            const tinyxml2::XMLElement * bndbox_child_xmax = bndbox->FirstChildElement ("xmax");
            const tinyxml2::XMLElement * bndbox_child_ymin = bndbox->FirstChildElement ("ymin");
            const tinyxml2::XMLElement * bndbox_child_ymax = bndbox->FirstChildElement ("ymax");
            
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
    
    
    
    array3i Xml::img_size() const
    {
        array3i size = make_array3i (0, 0, 0); //bunding boxs
        const tinyxml2::XMLElement* root = xml_doc_.RootElement();
        const tinyxml2::XMLElement* size_elem = root->FirstChildElement ("size");
        const tinyxml2::XMLElement * size_elem_child_width = size_elem->FirstChildElement ("width");
        const tinyxml2::XMLElement * size_elem_child_height = size_elem->FirstChildElement ("height");
        const tinyxml2::XMLElement * size_elem_child_depth = size_elem->FirstChildElement ("depth");
        
        if (size_elem_child_depth == nullptr || \
                size_elem_child_height == nullptr || \
                size_elem_child_width == nullptr)
        {
            EZLOG (Log_level::ERR) << "size error in :" << xml_file_path_;
            return size;
        }
        
        size[0] = atoi (size_elem_child_width->GetText());
        size[1] = atoi (size_elem_child_height->GetText());
        size[2] = atoi (size_elem_child_depth->GetText());
        return size;
    }
    
    
    std::vector<array4i> bbxs_in_xml (const std::string&xml_file_path)
    {
        Path path (xml_file_path);
        vector<array4i> result;
        
        if (path.exists())
        {
            ifstream in (xml_file_path);
            string str_buff, line;
            
            while (in >> line)
            {
                str_buff.append (line);
            }
            
            auto bbxs_str_vec (substrs (str_buff, "<bndbox>", "</bndbox>"));
            
            for (const auto &x : bbxs_str_vec)
            {
                auto xmin_vec = substrs (x, "<xmin>", "</xmin>");
                auto xmax_vec = substrs (x, "<xmax>", "</xmax>");
                auto ymin_vec = substrs (x, "<ymin>", "</ymin>");
                auto ymax_vec = substrs (x, "<ymax>", "</ymax>");
                
                if (xmin_vec.size() *xmax_vec.size() *ymin_vec.size() *ymax_vec.size() == 0)
                {
                    EZLOG (Log_level::ERR) << "bbx infos error " << xml_file_path;
                    continue;
                }
                
                int xmin = stoi (xmin_vec[0]);
                int xmax = stoi (xmax_vec[0]);
                int ymin = stoi (ymin_vec[0]);
                int ymax = stoi (ymax_vec[0]);
                result.push_back (make_array4i (xmin, ymin, xmax, ymax));
            }
            
            return result;
        }
        
        else
        {
            EZLOG (Log_level::ERR) << "can not open this file: " << xml_file_path;
            return result;
        }
    }
    
    //void Xml::create_xml_text_map (const tinyxml2::XMLElement*elem, const string path_prefx)
    //{
    //    if (elem == nullptr) { return; }
    //
    //    else
    //    {
    //        //elem is not empty and do not has text so it is a xml element
    //        if (elem->GetText() == nullptr)
    //        {
    //            create_xml_text_map (elem->FirstChildElement(), path_prefx + "." + elem->Name());
    //            auto next_sibling = elem->NextSiblingElement();
    //
    //            if (next_sibling != nullptr)
    //            {
    //                create_xml_text_map (next_sibling, path_prefx);
    //            }
    //        }
    //
    //        else
    //        {
    //xml_text_map_.insert(make_pair(path_prefx + "." + elem->Name(), elem->GetText()));
    //            auto next_sibling = elem->NextSiblingElement();
    //
    //            if (next_sibling != nullptr)
    //            {
    //                create_xml_text_map (next_sibling, path_prefx);
    //            }
    //        }
    //    }
    //}
}