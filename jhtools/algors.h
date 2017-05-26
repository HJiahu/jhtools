#ifndef ALGORIS_H_
#define ALGORIS_H_
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<functional>
#include<set>



namespace jhtools
{
#ifdef _NOEXCEPT
    #define noexcept _NOEXCEPT
#endif
    //*************使用RetainMaxNElems前需要为其中保存的元素重载运算符  >  ***************
    //也可以直接重载<  与 == 然后使用using namespase std::rel_ops  utility中的工具
    //下面使用两种方式实现RetainMaxNElems，分别是优先队列与堆，
    //在C++中优先队列也是使用堆实现的，但在当前场景下使用优先队列没有直接使用堆方便
    //从debug环境下的测试来看，还是第二种方式效率最低，第三种方式最好理解
    //#define PRIORITY_QUEUE
    //#define STL_MAKE_HEAP
#define STL_MULTISET
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
                
                else
                {
                    if (x > data_.top())
                    {
                        data_.push (x);
                        data_.pop();
                    }
                }
            }
            void push (T&&x)
            {
                if (data_.size() < max_size_) { data_.push (x); return; }
                
                else
                {
                    if (x > data_.top())
                    {
                        data_.push (x);
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
            
            std::vector<T> asc_order_data() const
            {
                return data();
            }
            
            int current_size() const
            {
                return data_.size();
            }
        private:
            std::priority_queue<T, std::vector<T>, std::greater<T> > data_;
            const unsigned int max_size_;
    };
#elif defined(STL_MAKE_HEAP)
    //下面使用小顶堆来实现
    template<typename T>
    struct RetainMaxNElems
    {
            RetainMaxNElems (unsigned int elem_num) : max_size_ (elem_num)
            {
                data_.reserve (max_size_);
            }
            void push (const T& x) noexcept
            {
                if (data_.size() < max_size_)
                {
                    data_.push_back (x);
                    std::push_heap (data_.begin(), data_.end(), std::greater<T>());
                    return;
                }
    
                else
                {
                    if (data_[0] < x)
                    {
                        assert (is_heap (data_.begin(), data_.end(), std::greater<T>()));
                        std::pop_heap (data_.begin(), data_.end(), std::greater<T>());
                        data_.back() = x;
                        std::push_heap (data_.begin(), data_.end(), std::greater<T>());
                    }
    
                    //while (data_.size() > max_size_){
                    //	std::pop_heap(data_.begin(), data_.end(), std::greater<T>());
                    //}
                }
            }
            void push (T&&x) noexcept
            {
                if (data_.size() < max_size_)
                {
                    data_.push_back (x);
                    std::push_heap (data_.begin(), data_.end(), std::greater<T>());
                    return;
                }
    
                else
                {
                    if (data_[0] < x)
                    {
                        std::pop_heap (data_.begin(), data_.end(), std::greater<T>());
                        data_.back() = x;
                        std::push_heap (data_.begin(), data_.end(), std::greater<T>());
                    }
    
                    //while (data_.size() > max_size_){
                    //	std::pop_heap(data_.begin(), data_.end(), std::greater<T>());
                    //}
                }
            }
            const std::vector<T> &data() const noexcept
            {
                return data_;
            }
            std::vector<T> asc_order_data() const noexcept
            {
                std::vector<T> buff = data_;
                std::sort_heap (buff.begin(), buff.end(), std::greater<T>());
                return vector<T> (buff.rbegin(), buff.rend());
            }
            int current_size() const noexcept
            {
                return data_.size();
            }
    
        private:
            std::vector<T> data_;
            const unsigned int max_size_;
    };
#elif defined(STL_MULTISET)  //使用multiset的实现
    template<typename T>
    struct RetainMaxNElems
    {
            RetainMaxNElems (unsigned int elem_num) : max_size_ (elem_num)
            {
                //data_.reserve (max_size_);
            }
            void push (const T& x)
            {
                if (data_.size() < max_size_)
                {
                    data_.insert (x);
                    return;
                }
    
                else
                {
                    if (*data_.begin() < x)
                    {
                        data_.insert (x);
                        data_.erase (data_.begin());
                    }
    
                    //while (data_.size() > max_size_){
                    //	std::pop_heap(data_.begin(), data_.end(), std::greater<T>());
                    //}
                }
            }
            void push (T&&x)
            {
                if (data_.size() < max_size_)
                {
                    data_.insert (x);
                    return;
                }
    
                else
                {
                    if (*data_.begin() < x)
                    {
                        data_.insert (x);
                        data_.erase (data_.begin());
                    }
                }
            }
            std::vector<T> data() const
            {
                std::vector<T> buff;
    
                for (const auto&x : data_)
                {
                    buff.push_back (x);
                }
    
                return buff;
            }
            std::vector<T> asc_order_data() const
            {
                return std::vector<T> (data_.begin(), data_.end());
            }
            int current_size() const
            {
                return data_.size();
            }
        private:
    
        private:
            std::multiset<T> data_;
            const unsigned int max_size_;
    };
#endif //PRIORITY_QUEUE
    
    
}

#endif // !ALGORIS_H_
