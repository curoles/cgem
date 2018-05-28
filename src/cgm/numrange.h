/**@file
 * @brief     num::range class for easy [n..m) looping with C++11 'for' 
 * @author    Igor Lesik
 * @copyright 2013-2015 Igor Lesik
 *
 * Usage examples:
 *
 * ```cpp
 *   for (auto i : num::range(5, 16, 3)) {
 *       cout << i << "\n";
 *   }
 *
 *   for (auto f : num::range(5.1, 8.3, 0.6)) {
 *       cout << f << "\n";
 *   }
 * ```
 *
 * @example numrange/test_numrange.cpp
 *
 * References:
 *  - http://xanduchene.wordpress.com/2013/03/17/pythonic-ranges-in-c11/
 *  - http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html
 *  - https://github.com/serge-sans-paille/pythran
 */
#pragma once
#ifndef CGM_NUMRANGE_H_INCLUDED
#define CGM_NUMRANGE_H_INCLUDED

#include <iterator>

namespace cgm::num {

/// Range implementation.
///
/// To work with `for` loop it implements methods
/// begin() and end() that both return an instance
/// of inner class iterator.
///
template<typename value_t>
class range_impl
{
    value_t rbegin;
    value_t rend;
    value_t step;
    std::size_t size_;
public:
    range_impl(value_t begin, value_t end, value_t step_arg):
        rbegin(begin), rend(end), step(step_arg)
    {
        if (rend < rbegin && step > 0) step = -step;
        if (rend > rbegin && step < 0) step = -step;
        if (step == 0) { rend = rbegin; size_ = 0; }
        else { size_ = (rend - rbegin)/step; }
        if ((rbegin + size_*step) != rend) {
            size_++;
        }
    }

    /// Range iterator keeps record of current iteration with
    /// variable `int current_step`. On each iteration variable
    /// current_value is adjusted with value of step.
    ///
    class iterator:
        public std::iterator<std::random_access_iterator_tag,value_t>
    {
        private:
            value_t current_value;
            int current_step;
            range_impl& parent;
        public:
            iterator(int start, range_impl& parent):
                current_step(start),
                parent(parent)
            {
                current_value = parent.rbegin + current_step*parent.step;
            }

            value_t operator*() { return current_value; }

            const iterator* operator++() {
                current_value += parent.step;
                current_step++;
                return this;
            }

            const iterator* operator++(int) {
                iterator old = *this;
                current_value += parent.step;
                current_step++;
                return old;
            }

            const iterator* operator--() {
                current_value -= parent.step;
                current_step--;
                return this;
            }

            iterator operator--(int){
                iterator old = *this;
                current_value -= parent.step;
                current_step--;
                return old;
            }

            bool operator==(const iterator& other) {
                return current_step == other.current_step;
            }

            bool operator!=(const iterator& other) {
                return current_step != other.current_step;
            }

            iterator operator+(int s) {
                iterator ret = *this;
                ret.current_step += s;
                ret.current_value += s*parent.step;
                return ret;
            }

            iterator operator-(int s){
                iterator ret = *this;
                ret.current_step -= s;
                ret.current_value -= s*parent.step;
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

/// Makes sequence a(n) = a(n-1) + step
///
template<typename vt,typename other>
auto range(other begin, other end, vt stepsize)->range_impl<decltype(begin+end+stepsize)>
{
    return range_impl<decltype(begin+end+stepsize)>(begin,end,stepsize);
}

/// Makes range [begin, end)
///
template<typename Tb,typename Te>
auto range(Tb begin, Te end) -> range_impl<decltype(begin+end)>
{
    return range_impl<decltype(begin+end)>(begin,end,1);
}

/// Makes range [0,end)
///
template<typename T>
auto range(T end) -> range_impl<T> {
    return range_impl<T>(0, end, 1);
}

} // namespace cgem::num

#endif
