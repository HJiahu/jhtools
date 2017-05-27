#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<gtest/gtest.h>
#include"jhtools/algors.h"

using namespace std;
using namespace jhtools;

vector<int> random_int;
vector<int> sorted_vec;

class AlgorsTest : public testing::Test
{
public:
        static void SetUpTestCase()
        {
			for (int i = 0; i < 1000; i++){
				random_int.push_back(rand()%1000 + 1);
			}
			sorted_vec = random_int;
			sort(sorted_vec.begin(), sorted_vec.end());
        }
        static void TearDownTestCase()
        {
        }
        static RetainMaxNElems<int> max_n_int_;
};

RetainMaxNElems<int> AlgorsTest::max_n_int_ (100);

//TEST_F(AlgorsTest, push){
//	for (auto x : random_int){
//		max_n_int_.push(x);
//	}
//	EXPECT_EQ(max_n_int_.data().size(),100);
//	vector<int> buff = max_n_int_.data();
//	sort(buff.begin(), buff.end());
//	EXPECT_EQ(vector<int>(sorted_vec.end() - 100, sorted_vec.end()), buff);
//}

//有一点需要注意，RetainMaxNElems中元素的个数不一定刚好等于设定的最大值
//因为输入数据的个数小于设定的最大元素个数
//TEST(RetainMaxNElems, actions){
//	const int case_num = 99;
//	for (int i = 0; i < case_num; i++){
//		vector<int> buff;
//		int retain_num = rand() % 100 + 1;
//		int buff_size = rand()+retain_num+10;
//		for (int j = 0; j < buff_size; j++){
//			buff.push_back(rand());
//		}
//		assert(buff.size() > retain_num);
//		RetainMaxNElems<int> retain_max_n(retain_num);
//		for (auto x : buff){
//			retain_max_n.push(x);
//		}
//		auto result_buff = retain_max_n.data();
//		assert(result_buff.size() == retain_max_n.current_size());
//		sort(result_buff.begin(), result_buff.end());
//		sort(buff.begin(), buff.end());
//		auto buffff = vector<int>(buff.end() - retain_num, buff.end());
//		EXPECT_TRUE(buffff.size() == retain_num);
//		EXPECT_TRUE(result_buff.size() == retain_num);
//		EXPECT_EQ(retain_max_n.current_size() , retain_num);
//		EXPECT_EQ(buffff, result_buff);
//		EXPECT_EQ(buffff,retain_max_n.asc_order_data());
//	}
//}