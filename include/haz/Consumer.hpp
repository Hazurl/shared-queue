#pragma once

#include <haz/SharedQueue.hpp>
#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S>
class Consumer {
public:

    Consumer(SharedQueue<T, S>& queue) : queue(queue) {}

    T pop() {
        /*
        while(true) {
            queue.lock();

            if (!queue.empty())
                break;

            queue.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds{10});
            
        }*/

        auto lock = queue.acquire_lock();
        queue.wait_not_empty(lock);

        T value = queue.top();
        queue.pop();

        //queue.unlock();
        lock.unlock();

        scout << "Pop     " << value << ", " << queue.size() << "/" << queue.capacity() << " elements in queue\n";
        return value;
    }

private:

    SharedQueue<T, S>& queue;

};

}