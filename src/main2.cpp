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
#include <type_traits>

#include <haz/SharedQueue.hpp>
#include <haz/Queue.hpp>
#include <haz/Producer.hpp>
#include <haz/Consumer.hpp>
#include <haz/Out.hpp>
#include <haz/Policy.hpp>


struct Observer {
    constexpr Observer() noexcept {
        std::cout << "[Default Constructor]\n";
    }

    ~Observer() noexcept {
        std::cout << "[Destructor]\n";
    }

    Observer(Observer const&) noexcept {
        std::cout << "[Copy Constructor]\n";
    }

    Observer(Observer&&) noexcept {
        std::cout << "[Move Constructor]\n";
    }

    Observer& operator=(Observer const&) noexcept {
        std::cout << "[Copy operator=]\n";
        return *this;
    }

    Observer& operator=(Observer&&) noexcept {
        std::cout << "[Move operator=]\n";
        return *this;
    }

};

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
    std::cout << "#\n";

    for(auto it = queue.rbegin(); it != queue.rend(); ++it) {
        std::cout << *it << ", ";
    }
    std::cout << "#\n";
    std::cout << "#\n";

    haz::Queue<int, size_max> other;
    other.push_back(123456);
    for(auto const& i : other) {
        std::cout << i << ", ";
    }
    std::cout << "#\n";
    for(auto const& i : queue) {
        std::cout << i << ", ";
    }
    std::cout << '\n' << "swap!\n";
    std::swap(queue, other);
    for(auto const& i : other) {
        std::cout << i << ", ";
    }
    std::cout << "#\n";
    for(auto const& i : queue) {
        std::cout << i << ", ";
    }
    std::cout << '\n' << "swap!\n";
    queue.swap(other);
    for(auto const& i : other) {
        std::cout << i << ", ";
    }
    std::cout << "#\n";
    for(auto const& i : queue) {
        std::cout << i << ", ";
    }
    std::cout << "#\n";
    std::cout << "#\n";
    
    other.clear();
    other.emplace_back(10);
    other.push_back(11);
    other.push_back(12);
    other.push_back(13);

    std::cout << "a < b:  " << (queue < other) << '\n';
    std::cout << "a <= b: " << (queue <= other) << '\n';
    std::cout << "a >= b: " << (queue >= other) << '\n';
    std::cout << "a > b:  " << (queue > other) << '\n';
    std::cout << "a == b: " << (queue == other) << '\n';
    std::cout << "a != b: " << (queue != other) << '\n';

    std::cout << "================================\n";

    {
        std::array<Observer, 1> a{ Observer{} };
        std::cout << "#\n";
        std::array<Observer, 1> b(std::move(a));
        std::cout << "#\n";
        std::array<Observer, 1> c(b);
        std::cout << "#\n";
        b = c;
        std::cout << "#\n";
        c = std::move(b);
        std::cout << "#\n";
    }

    std::cout << "\n";

    {
        std::vector<Observer> a{ Observer{} };
        std::cout << "#\n";
        std::vector<Observer> b(std::move(a));
        std::cout << "#\n";
        std::vector<Observer> c(b);
        std::cout << "#\n";
        b.reserve(2);
        std::cout << "-\n";
        c.reserve(2);
        std::cout << "-\n";
        c.emplace_back();
        std::cout << "-\n";
        b = c;
        std::cout << "#\n";
        c = std::move(b);
        std::cout << "#\n";
    }

    std::cout << "\n";
    
    {
        haz::Queue<Observer, 1> a{ Observer{} };
        std::cout << "#\n";
        haz::Queue<Observer, 1> b(std::move(a));
        std::cout << "#\n";
        haz::Queue<Observer, 1> c(b);
        std::cout << "#\n";
        b = c;
        std::cout << "#\n";
        c = std::move(b);
        std::cout << "#\n";
    }

    std::cout << "\n";
}

