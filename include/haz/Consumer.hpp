#pragma once

#include <haz/SharedQueue.hpp>
#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S>
class Consumer {
public:

    Consumer(SharedQueue<T, S>& queue) : queue(queue) {}

    T pop() {
        auto lock = queue.wait_not_empty();

        T value = queue.top();
        queue.pop();

        lock.unlock();

        scout << "Pop     " << value << ", " << queue.size() << "/" << queue.capacity() << " elements in queue\n";
        return value;
    }

private:

    SharedQueue<T, S>& queue;

};

}