#include <queue>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <random>
#include <chrono>
#include <condition_variable>
#include <atomic>
#include <chrono>

#include <haz/SharedQueue.hpp>
#include <haz/Queue.hpp>
#include <haz/Producer.hpp>
#include <haz/Consumer.hpp>
#include <haz/Out.hpp>
#include <haz/Policy.hpp>


static constexpr std::size_t size_max = 5;

int main() {
    haz::Queue<int, size_max> queue;
    std::cout << queue.size() << "/" << queue.capacity() << "=" << queue.max_size() << ", " << queue.empty() << ", " << queue.full() << '\n';

    queue.push_back(42);
    queue.emplace_back(10);
    { int i = 666; queue.push_back(i); }

    std::cout << queue.size() << "/" << queue.capacity() << "=" << queue.max_size() << ", " << queue.empty() << ", " << queue.full() << '\n';

    queue.pop_front();
    queue.pop_front();

    std::cout << queue.size() << "/" << queue.capacity() << "=" << queue.max_size() << ", " << queue.empty() << ", " << queue.full() << '\n';

    queue.pop_front();

    std::cout << queue.size() << "/" << queue.capacity() << "=" << queue.max_size() << ", " << queue.empty() << ", " << queue.full() << '\n';

    queue.emplace_back(10) = 169;
    queue.emplace_back(1337);
    queue.push_back(42);
    queue.push_back(-1);
    { int i = 666; queue.push_back(i); }

    std::cout << queue.size() << "/" << queue.capacity() << "=" << queue.max_size() << ", " << queue.empty() << ", " << queue.full() << '\n';

    std::cout << queue.front() << ", " << queue.back() << '\n';
    
    queue.pop_front();

    std::cout << queue.front() << ", " << queue.back() << '\n';
    std::cout << queue.at(2) << ", " << queue[2] << '\n';

    std::cout << queue.size() << "/" << queue.capacity() << "=" << queue.max_size() << ", " << queue.empty() << ", " << queue.full() << '\n';

    queue.clear();

    std::cout << queue.size() << "/" << queue.capacity() << "=" << queue.max_size() << ", " << queue.empty() << ", " << queue.full() << '\n';

    queue.emplace_back(10);
    queue.push_back(11);
    queue.push_back(12);
    queue.push_back(13);

    std::cout << queue.size() << "/" << queue.capacity() << "=" << queue.max_size() << ", " << queue.empty() << ", " << queue.full() << '\n';

    for(auto const& i : queue) {
        std::cout << i << ", ";
    }
    std::cout << '\n';

    for(auto it = queue.rbegin(); it != queue.rend(); ++it) {
        std::cout << *it << ", ";
    }
    std::cout << '\n';

}

