#include<gtest/gtest.h>
#include"jhtools/utils.h"
#include<string>

using namespace std;
using namespace jhtools;

extern string test_data_path;

TEST (utils, replace_tail)
{
    //wstring str_1(L"this is a test.test");
    //wstring str_2(L"this is a test-test");
    auto assist_replace_fun = [] (string str, const string & old_str, const string & new_str)->string
    {
        replace_tail (str, old_str, new_str);
        return str;
    };
    const string str_2c ("");
    const string str_3c ("haha.xyz");
    const string str_4c ("abc.defghij");
    //test empty string
    //EXPECT_FALSE (replace_tail (string (str_2c), "", "xyz"));
    //xy is not tail
    EXPECT_EQ (str_3c, assist_replace_fun (str_3c, "xy", "zyx"));
    //target string is empty
    EXPECT_NE (assist_replace_fun (str_3c, "xhaha.xyz", "zyx"), "xzyx");
    EXPECT_NE (assist_replace_fun (str_3c, "xhaha.xyz", "zyx"), "zyx");
    EXPECT_EQ (assist_replace_fun (str_3c, "yz", "zyx"), "haha.xzyx");
    EXPECT_EQ (assist_replace_fun (str_3c, ".xyz", ".z"), "haha.z");
}

TEST (endwith, all)
{
    EXPECT_TRUE (endwith ("this is bad", "bad"));
    EXPECT_FALSE (endwith ("this is a bad", "ba"));
}


TEST (startwith, all)
{
    EXPECT_TRUE (startwith ("this is bad", "th"));
    EXPECT_FALSE (startwith ("this is a bad", "hi"));
}

TEST (file, copy)
{
    EXPECT_FALSE (copy_file ("E:/jhtools/test/imgs", "E:/jhtools/test/imgs"));
    EXPECT_FALSE (copy_file ("", "E:/jhtools/test/imgs"));
    EXPECT_FALSE (copy_file ("E:/jhtools/test/imgs/nonface0.png", "E:/jhtools/test/imgs"));
}


TEST (substrs, all)
{
    string test_string_1 = R"(<name>head</name>
		<pose>Unspecified< / pose>
		<truncated>0< / truncated>
		<difficult>0< / difficult>
		<bndbox>
		<xmin>500</xmin>
		<ymin>190</ymin>
		<xmax>592</xmax>
		<ymax>283</ymax>
		</bndbox>)";
    auto result = substrs (test_string_1, "<bndbox>", "</bndbox>");
    EXPECT_EQ (result.size(), 1);
    auto in_xmin = substrs (result[0], "<xmin>", "</xmin>");
    EXPECT_EQ (in_xmin[0], "500");
    auto in_ymax = substrs (result[0], "<ymax>", "</ymax>");
    EXPECT_EQ (in_ymax[0], "283");
    string test_string_2 = R"(
  <object>
    <name>head</name>
    <pose>Unspecified</pose>
    <truncated>0</truncated>
    <difficult>0</difficult>
    <bndbox>
      <xmin>322</xmin>
      <ymin>81</ymin>
      <xmax>422</xmax>
      <ymax>186</ymax>
    </bndbox>
  </object>
  <object>
    <name>head</name>
    <pose>Unspecified</pose>
    <truncated>0</truncated>
    <difficult>0</difficult>
    <bndbox>
      <xmin>364</xmin>
      <ymin>286</ymin>
      <xmax>471</xmax>
      <ymax>393</ymax>
    </bndbox>
  </object>
  <object>
    <name>head</name>
    <pose>Unspecified</pose>
    <truncated>0</truncated>
    <difficult>0</difficult>
    <bndbox>
      <xmin>547</xmin>
      <ymin>212</ymin>
      <xmax>640</xmax>
      <ymax>309</ymax>
    </bndbox>
  </object>
</annotation>
	)";
    result = substrs (test_string_2, "<bndbox>", "</bndbox>");
    EXPECT_EQ (result.size(), 3);
    in_xmin = substrs (result[1], "<xmin>", "</xmin>");
    EXPECT_EQ (in_xmin[0], "364");
    in_ymax = substrs (result[2], "<ymax>", "</ymax>");
    EXPECT_EQ (in_ymax[0], "309");
}

TEST (stojson, fun)
{
    string j_str (R"({
"china":960,
"canada":998
}
)");
	auto jn = stojson(j_str);
	//cout << jn["china"].int_value() << endl;
	EXPECT_EQ(jn["china"].int_value(),960);
	//只有类型对应正确才能获得正确的结果
	EXPECT_EQ(jn["china"].string_value().size(), 0);

}

TEST(ftojson, fun){

	auto jn = ftojson(path(test_data_path+"/json.txt").string());
    EXPECT_EQ (jn["china"].int_value(), 960);
    //只有类型对应正确才能获得正确的结果
    EXPECT_EQ (jn["china"].string_value().size(), 0);
}
