#pragma once

#include <array>
#include <cstddef>
#include <mutex>

#include <haz/Policy.hpp>

namespace haz {

template<typename T, std::size_t S>
class SharedQueue {
public:

    std::unique_lock<std::mutex> acquire_lock() { return std::unique_lock(mutex); }

    std::unique_lock<std::mutex> wait_not_empty() { 
        return wait_not_empty(acquire_lock());
    }
    std::unique_lock<std::mutex> wait_not_empty(std::unique_lock<std::mutex> lock) { 
        if (!interrupt)
            not_empty_cv.wait(lock, [this] () { return !this->empty() || interrupt.load(); }); 
        if (interrupt) {
            lock.unlock();
            throw std::runtime_error("Interrupt thread!");
        }
        return std::move(lock);
    }
    std::unique_lock<std::mutex> wait_not_full() { 
        return wait_not_full(acquire_lock());
    }
    std::unique_lock<std::mutex> wait_not_full(std::unique_lock<std::mutex> lock) { 
        if (!interrupt)
            not_full_cv.wait(lock, [this] () { return this->size() < this->capacity() || interrupt.load(); });
        if (interrupt) {
            lock.unlock();
            throw std::runtime_error("Interrupt thread!");
        }
        return std::move(lock);
    }

    void interrupt_all() {
        interrupt.store(true);
        not_full_cv.notify_all();
        not_empty_cv.notify_all();
    }

    T&       top()       { return data[first]; };
    T const& top() const { return data[first]; };

    void pop() {
        --cur_size;
        first = (first + 1) % S;
        not_full_cv.notify_one();
    }

    void push(T const& t) {
        if (cur_size >= S)
            return;

        ++cur_size;
        data[last] = t;
        last = (last + 1) % S;
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

    std::array<T, S> data;
    std::size_t first{0};
    std::size_t last{0};
    std::size_t cur_size{0};

    std::mutex mutex;
    std::atomic_bool interrupt{false};
    std::condition_variable not_empty_cv;
    std::condition_variable not_full_cv;

};

}