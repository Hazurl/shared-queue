#pragma once

#include <haz/SharedQueue.hpp>
#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S, typename P = StandardPolicy>
class Producer {
public:

    Producer(SharedQueue<T, S>& queue) : queue(&queue) {}

    void push(T const& value) {
        auto lock = queue->wait_not_full();

        queue->push(value);

        lock.unlock();
    }

private:

    SharedQueue<T, S>* queue;

};

template<typename T, std::size_t S, std::size_t B>
class Producer<T, S, Buffered<B>> {
public:

    Producer(SharedQueue<T, S>& queue) : queue(&queue) {}

    void push(T const& value) {
        buffer[count++] = value;
        if (count >= B) 
            clean();
    }

private:

    void clean() {
        auto lock = queue->wait_not_full();

        while(count > 0 && queue->size() < queue->capacity())
            queue->push(buffer[--count]);

        lock.unlock();
    }

    std::array<T, B> buffer;
    std::size_t count{0};
    SharedQueue<T, S>* queue;

};

}