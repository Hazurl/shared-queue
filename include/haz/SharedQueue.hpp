#pragma once

#include <array>
#include <cstddef>
#include <mutex>

namespace haz {

template<typename T, std::size_t S>
class SharedQueue {
public:
/*
    void lock() { return mutex.lock(); }
    void unlock() { return mutex.unlock(); }
    bool try_lock() { return mutex.try_lock(); }
*/
    std::unique_lock<std::mutex> acquire_lock() { return std::unique_lock(mutex); }
    void wait_not_empty(std::unique_lock<std::mutex>& lock) { return not_empty_cv.wait(lock, [this] () { return !this->empty(); }); }
    void wait_not_full (std::unique_lock<std::mutex>& lock) { return not_full_cv .wait(lock, [this] () { return this->size() < this->capacity(); }); }

    T&       top()       { return data[first]; };
    T const& top() const { return data[first]; };

    void pop() {
        --cur_size;
        first = wrap(first + 1);
        not_full_cv.notify_one();
    }

    void push(T const& t) {
        if (cur_size >= S)
            return;

        ++cur_size;
        data[last] = t;
        last = wrap(last + 1);
        not_empty_cv.notify_one();
    }

    template<typename I>
    void push_all(I begin, I end) {
        for(; begin != end && cur_size < S; ++begin)
            push(*begin);
    }

    template<typename...Args>
    void emplace(Args&&... args) {
        if (cur_size >= S)
            return;

        ++cur_size;
        data[last] = T { std::forward<Args&&>(args)... };
        last = wrap(last + 1);
        not_empty_cv.notify_one();
    }

    std::size_t size() const {
        return cur_size;
    }

    bool empty() const {
        return cur_size == 0;
    }

    constexpr std::size_t capacity() const {
        return S;
    }

private:

    static std::size_t wrap(std::size_t n) {
        if constexpr ((S & (S - 1)) == 0) { // is power of two
            return n & (S - 1);
        } else {
            return n % S;
        }
    }

    std::array<T, S> data;
    std::size_t first{0};
    std::size_t last{0};
    std::size_t cur_size{0};

    std::mutex mutex;
    std::condition_variable not_empty_cv;
    std::condition_variable not_full_cv;

};

}