#ifndef ALGORIS_H_
#define ALGORIS_H_
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<functional>



namespace jhtools
{
	//*************ʹ��RetainMaxNElemsǰ��ҪΪ���б����Ԫ�����������  >  ***************
    //����ʹ�����ַ�ʽʵ��RetainMaxNElems���ֱ������ȶ�����ѣ�
    //��C++�����ȶ���Ҳ��ʹ�ö�ʵ�ֵģ����ڵ�ǰ������ʹ�����ȶ���û��ֱ��ʹ�öѷ���
#define PRIORITY_QUEUE
#ifdef PRIORITY_QUEUE    //else use heap 
    //����Ľṹ�����ڱ���M������������N������>=N��
    //ʹ�����ȶ�������������
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
    //����ʹ��С������ʵ��
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
