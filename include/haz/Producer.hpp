#pragma once

#include <haz/SharedQueue.hpp>
#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S>
class Producer {
public:

    Producer(SharedQueue<T, S>& queue) : queue(queue) {}

    void push(T const& value) {
        /*
        while(true) {
            queue.lock();

            if (queue.size() < queue.capacity())
                break;

            queue.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds{10});
        }*/

        auto lock = queue.acquire_lock();
        queue.wait_not_full(lock);

        queue.push(value);

        lock.unlock();
        //queue.unlock();

        scout << "Insert  " << value << ", " << queue.size() << "/" << queue.capacity() << " elements in queue\n";
    }

private:

    SharedQueue<T, S>& queue;

};

}