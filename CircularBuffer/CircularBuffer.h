/**@file
 * @brief     Circular Buffer
 * @author    Igor Lesik
 * @copyright 2010-2015 Igor Lesik
 *
 */
#pragma once

#ifndef CGEM_CIRCULAR_BUFFER_H_INCLUDED
#define CGEM_CIRCULAR_BUFFER_H_INCLUDED

#include <cstddef>
#include <array>
#include <atomic>
#include <cassert>
#include <mutex>
#include <condition_variable>

namespace gem {

template<typename T, std::size_t SIZE=64>
class CircularBuffer
{
private:
    typedef std::array<T,SIZE> Array;
public:
    typedef std::size_t size_type;
    typedef T value_type;
    typedef T& reference;

public:
    CircularBuffer(const CircularBuffer&) =delete;
    CircularBuffer(CircularBuffer&&) =delete;
    CircularBuffer():push_ptr_(0),pop_ptr_(0){
    }

    constexpr size_type max_size() const { return SIZE; }

    size_type size() {
        return push_ptr_.load() - pop_ptr_.load();
    }

    bool empty() const {
        return pop_ptr_ == push_ptr_;
    }

    bool full() {
        return size() >= max_size();
    }

    /**
     * @return false if buffer is full.
     */  
    bool push_back(const T& item) {
        if (full()) return false;
        buffer_[push_ptr_ % SIZE] = item;
        push_ptr_++;
        data_available_event_.notify_one();
        return true;
    }

    //TODO TODO
    //bool push_back(T&& item) {
    //}

    /** Get next item.
     *
     * Call empty() to check if pop() is allowed.
     */
    T pop() {
        assert(pop_ptr_ < push_ptr_);
        T item = buffer_[pop_ptr_ % SIZE];
        pop_ptr_++;
        return item;
    }

    T& front() {
        return buffer_[pop_ptr_ % SIZE];
    }

    //@return std::cv_status::timeout or std::cv_status::no_timeout
    std::cv_status wait_for(uint64_t timeout_millisec) {
        std::unique_lock<std::mutex> lock(event_mutex_);
        return data_available_event_.wait_for(lock, std::chrono::milliseconds(timeout_millisec));
    }

private:
    Array buffer_;

    std::atomic<std::size_t> push_ptr_;
    std::atomic<std::size_t> pop_ptr_;

    std::mutex event_mutex_;
    std::condition_variable data_available_event_;
};

} // end of namespace gem

#endif
