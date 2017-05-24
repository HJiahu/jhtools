#ifndef ALGORIS_H_
#define ALGORIS_H_
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<functional>



namespace jhtools
{
	//*************使用RetainMaxNElems前需要为其中保存的元素重载运算符  >  ***************
    //下面使用两种方式实现RetainMaxNElems，分别是优先队列与堆，
    //在C++中优先队列也是使用堆实现的，但在当前场景下使用优先队列没有直接使用堆方便
#define PRIORITY_QUEUE
#ifdef PRIORITY_QUEUE    //else use heap 
    //下面的结构体用于保存M个数据中最大的N个（Ｍ>=N）
    //使用优先队列来保存数据
    template<typename T>
    struct RetainMaxNElems
    {
            RetainMaxNElems (unsigned int elem_num) : max_size_ (elem_num)
            {
                //data_.reserve(max_size_);
            }
            void push (const T& x)
            {
                if (data_.size() < max_size_) { data_.push (x); return; }
                
				else{
					if (x > data_.top())
					{
						data_.push(x);
					}

					while (data_.size() > max_size_)
					{
						data_.pop();
					}
				}

            }
            void push (T&&x)
            {
                if (data_.size() < max_size_) { data_.push (x); return; }
                
				else{
					if (x > data_.top())
					{
						data_.push(x);
					}

					while (data_.size() > max_size_)
					{
						data_.pop();
					}
				}

            }
            std::vector<T> data() const
            {
                auto buff = data_;
                vector<T> vec;
                
                while (!buff.empty())
                {
                    vec.push_back (buff.top());
                    buff.pop();
                }
                
                return vec;
            }
            
            int current_size() const
            {
                return data_.size();
            }
        private:
            std::priority_queue<T, std::vector<T>, std::greater<T> > data_;
            const unsigned int max_size_;
    };
#else
    //下面使用小顶堆来实现
    template<typename T>
    struct RetainMaxNElems
    {
            RetainMaxNElems (unsigned int elem_num) : max_size_ (elem_num)
            {
                data_.reserve (max_size_);
            }
            void push (const T& x)
            {
                if (data_.size() > max_size_)
                {
                    data_.pop();
                }
    
                if (data_.size() == 0) { data_.push (x); }
    
                else
                    if (x > data_.top())
                    {
                        data_.push (x);
                    }
            }
            void push (T&&x)
            {
                if (data_.size() > max_size_)
                {
                    data_.pop();
                }
    
                if (data_.size() == 0) { data_.push (x); }
    
                else
                    if (x > data_.top())
                    {
                        data_.push (x);
                    }
            }
            std::vector<T> data() const
            {
                auto buff = data_;
                int size = buff.size();
                vector<T> vec;
    
                for (int i = 0; i < size; i++)
                {
                    vec.push_back (buff.top());
                    buff.pop();
                }
    
                return vec;
            }
            int size() const
            {
                return data_.size();
            }
        private:
    
        private:
            std::vector<T> data_;
            const unsigned int max_size_;
    };
#endif //PRIORITY_QUEUE
    
    
}

#endif // !ALGORIS_H_
