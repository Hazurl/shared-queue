#pragma once

#include <haz/SharedQueue.hpp>
#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S>
class Producer {
public:

    Producer(SharedQueue<T, S>& queue) : queue(queue) {}

    void push(T const& value) {
        auto lock = queue.wait_not_full();

        queue.push(value);

        lock.unlock();

        scout << "Insert  " << value << ", " << queue.size() << "/" << queue.capacity() << " elements in queue\n";
    }

private:

    SharedQueue<T, S>& queue;

};

}