/**@file
 * @brief Array allocated on stack
 * @author Igor Lesik 2018
 *
 * This implementation uses C/C++ extention called Variable-length Automatic Arrays.
 * [GNU VLA](https://gcc.gnu.org/onlinedocs/gcc/Variable-Length.html)
 *
 * If VLA is not available, `stack_array` will use dynamic memory allocation
 * with std::unique_ptr.
 *
 * Even crude experiment using `time a.out`  with allocating significant chunk
 * of memory with `stack_array(int,1000000,a);` shows that
 * VLA is faster than dynamic memory allocation.
 *
 *
 */
#pragma once
#ifndef CGM_STACK_ARRAY_H_INCLUDED
#define CGM_STACK_ARRAY_H_INCLUDED

#include <cstddef> // size_t
#include <cassert> // assert
#include <memory>  // unique_ptr

namespace cgm {

template <class T, class = std::enable_if_t<std::is_pointer<T>::value>>
using not_null = T;

/** STL-like adapter for C-style raw array
 *
 * Takes a pointer to a raw array and size of the array,
 * implements std::array like interface.
 */
template<typename T>
class raw_ptr_array
{
    const std::size_t size_; ///< size of the allocated array
    T* const ptr_;           ///< non-owning pointer to raw array

public:
    raw_ptr_array(size_t size, not_null<T*> ptr):size_(size),ptr_(ptr){
        assert(size > 0 && ptr != nullptr);
    }

    raw_ptr_array(const raw_ptr_array&) = delete;
    raw_ptr_array(raw_ptr_array&&) = delete;

    T& operator[](std::size_t n) {
        assert(n < size_);
        return ptr_[n];
    }

    const T& operator[](std::size_t n) const {
        assert(n < size_);
        return ptr_[n];
    }

    std::size_t size() const { return size_; }

    constexpr T* data() noexcept { return ptr_; }

    T* begin() { return ptr_; }
    T* end() { return ptr_ + size_; }
};

 
/// Create 2 objects: 1) array `T p[n]` and 2) `raw_ptr_array<T[]> a(p)`. 
///
#define CGM_STACK_ARRAY_VLA(T,size,name) \
    T name##_vla_[size]; \
    cgm::raw_ptr_array<T> name(size, name##_vla_);
 
#define CGM_STACK_ARRAY_UPTR(T,size,name) \
    std::unique_ptr<T[]> name##_unqptr_ = std::make_unique<T[]>(size); \
    cgm::raw_ptr_array<T> name(size, name##_unqptr_.get());

/// Macro that hides...
///
#if defined(__GNUC__)
    #pragma Using Variable-length Automatic Arrays for stack_array
    #define stack_array(T,size,name) CGM_STACK_ARRAY_VLA(T,size,name)
#else
    #define stack_array(T,size,name) CGM_STACK_ARRAY_UPTR(T,size,name)
#endif

} // namespace cgm

#endif 

