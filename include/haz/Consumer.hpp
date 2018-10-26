#pragma once

#include <haz/SharedQueue.hpp>
#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S, typename P = StandardPolicy>
class Consumer {
public:

    Consumer(SharedQueue<T, S>& queue) : queue(&queue) {}

    T pop() {
        auto lock = queue->wait_not_empty();

        T value = queue->top();
        queue->pop();

        lock.unlock();
        return value;
    }

private:

    SharedQueue<T, S>* queue;

};


template<typename T, std::size_t S, std::size_t B>
class Consumer<T, S, Buffered<B>> {
public:

    Consumer(SharedQueue<T, S>& queue) : queue(&queue) {}

    T pop() {
        if (count <= 0)
            fill();

        return buffer[--count];
    }

private:

    void fill() {
        auto lock = queue->wait_not_empty();

        while(count < B && !queue->empty()) {
            buffer[count++] = queue->top();
            queue->pop();
        }

        lock.unlock();
    }

    std::array<T, B> buffer;
    std::size_t count{0};
    SharedQueue<T, S>* queue;

};

}