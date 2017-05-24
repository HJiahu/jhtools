#include"jhtools/xml.h"
#include<string>
#include<gtest/gtest.h>

using namespace jhtools;
using namespace std;

static const string xml_path ("E:/jhtools/test/xml_files");

TEST (xml_helpers, make_array4i)
{
    array4i x, y;
    x[0] = 1;
    x[1] = 4;
    x[2] = 2;
    x[3] = 9;
    //
    y[0] = 8;
    y[1] = 99;
    y[2] = 41651;
    y[3] = 88;
    EXPECT_EQ (x, make_array4i (1, 4, 2, 9));
    EXPECT_NE (x, make_array4i (1, 4, 2, 99));
    EXPECT_EQ (y, make_array4i (8.0, 99.0, 41651.0, 88.0));
}


class XmlTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            xml_ = new Xml (xml_path + "/mov_004_020952.xml");
        }
        
        static void TearDownTestCase()
        {
            delete xml_;
        }
        
    public:
        static Xml *xml_;
};
Xml *XmlTest::xml_;

TEST_F (XmlTest, bboxs)
{
    auto bbs = xml_->bboxs();
    EXPECT_EQ (bbs.size(), 2);
    EXPECT_EQ (bbs[0], make_array4i (200, 112, 229, 142));
    EXPECT_NE (bbs[0], make_array4i (200, 11, 229, 142));
    EXPECT_EQ (bbs[1], make_array4i (311, 49, 394, 131));
}

TEST_F(XmlTest, load_xml_file){
	xml_->load_xml_file(xml_path + "/mov_010_025673_u.xml");
	auto bbs = xml_->bboxs();

	EXPECT_EQ(bbs.size(), 1);
	EXPECT_EQ(bbs[0], make_array4i(133, 240, 200, 320));
	EXPECT_NE(bbs[0], make_array4i(200, 11, 229, 142));
}

TEST_F(XmlTest, img_size){
	xml_->load_xml_file(xml_path + "/mov_010_025673_u.xml");
	auto size = xml_->img_size();
	EXPECT_EQ(size, make_array3i(544,416,3));
	xml_->load_xml_file(xml_path + "/mov_001_007623.xml");
	size = xml_->img_size();
	EXPECT_EQ(size, make_array3i(528, 224, 3));
}

TEST_F(XmlTest, bbxs_in_xml){
	auto bbs = bbxs_in_xml(xml_path + "/mov_010_025673_u.xml");

	EXPECT_EQ(bbs.size(), 1);
	EXPECT_EQ(bbs[0], make_array4i(133, 240, 200, 320));
	EXPECT_NE(bbs[0], make_array4i(200, 11, 229, 142));
}