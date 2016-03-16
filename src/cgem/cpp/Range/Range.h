/**@file
 * @brief     Range class for easy looping with C++11 'for' 
 * @author    Igor Lesik
 * @copyright 2013-2015 Igor Lesik
 *
 *   Usage:
 *
 *   for (auto i : range(5, 16, 3))
 *   {
 *       cout << i << "\n";
 *   }
 *
 *   for (auto f : range(5.1, 8.3, 0.6))
 *   {
 *       cout << f << "\n";
 *   }
 *
 * http://xanduchene.wordpress.com/2013/03/17/pythonic-ranges-in-c11/
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html
 * https://github.com/serge-sans-paille/pythran
 */
#pragma once

#include <iterator>

namespace cgem {

template<typename value_t>
class range_impl
{
private:
    value_t rbegin;
    value_t rend;
    value_t step;
    std::size_t size_;
public:
    range_impl(value_t begin, value_t end, value_t step=1):
        rbegin(begin), rend(end), step(step)
   {
        size_ = (rend - rbegin)/step;
        if(rbegin+size_*step != rend){
            size_++;
        }
    }

    class iterator:
        public std::iterator<std::random_access_iterator_tag,value_t>
    {
        private:
            value_t current_value;
            int current_step;
            range_impl& parent;
        public:
            iterator(int start,range_impl& parent): current_step(start), parent(parent){current_value=parent.rbegin+current_step*parent.step;}
            value_t operator*() {return current_value;}
            const iterator* operator++(){
                current_value+=parent.step;
                current_step++;
                return this;
            }
            const iterator* operator++(int){
                current_value+=parent.step;
                current_step++;
                return this;
            }
            const iterator* operator--(){
                current_value-=parent.step;
                current_step--;
                return this;}
            iterator operator--(int){
                iterator old=*this;
                current_value-=parent.step;
                current_step--;
                return old;
            }
            bool operator==(const iterator& other) {
                return current_step==other.current_step;
            }
            bool operator!=(const iterator& other) {
                return current_step!=other.current_step;
            }
            iterator operator+(int s) {
                iterator ret=*this;
                ret.current_step+=s;
                ret.current_value+=s*parent.step;
                return ret;
            }
            iterator operator-(int s){
                iterator ret=*this;
                ret.current_step-=s;
                ret.current_value-=s*parent.step;
                return ret;
            }
    };

    iterator begin(){
        return iterator(0, *this);
    }

    iterator end(){
        return iterator(size_, *this);
    }

    value_t operator[](int index){
        return rbegin + index*step;
    }

    std::size_t size(){
        return size_;
    }
};

template<typename vt,typename other>
auto range(other begin, other end, vt stepsize)->range_impl<decltype(begin+end+stepsize)>
{
    return range_impl<decltype(begin+end+stepsize)>(begin,end,stepsize);
}

template<typename Tb,typename Te>
auto range(Tb begin, Te end) -> range_impl<decltype(begin+end)>
{
    return range_impl<decltype(begin+end)>(begin,end,1);
}

template<typename T>
range_impl<T> range(T end){
    return range_impl<T>(0, end, 1);
}

} // namespace cgem
