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

static unsigned obs_ctr{0};
static unsigned obs_dtr{0};
static unsigned obs_agn{0};

struct Observer {
    constexpr Observer() noexcept {
        std::cout << "[Default Constructor]\n";
        ++obs_ctr;
    }

    ~Observer() noexcept {
        std::cout << "[Destructor]\n";
        if (obs_dtr >= obs_ctr) {
            std::cerr << "###########################\n/!\\ WTF ??? destructed more time than constructed\n";
        }
        ++obs_dtr;
    }

    Observer(Observer const&) noexcept {
        std::cout << "[Copy Constructor]\n";
        ++obs_ctr;
    }

    Observer(Observer&&) noexcept {
        std::cout << "[Move Constructor]\n";
        ++obs_ctr;
    }

    Observer& operator=(Observer const&) noexcept {
        std::cout << "[Copy operator=]\n";
        ++obs_agn;
        return *this;
    }

    Observer& operator=(Observer&&) noexcept {
        std::cout << "[Move operator=]\n";
        ++obs_agn;
        return *this;
    }

};

constexpr int foo() {
    haz::Queue<int, 5> queue(4, 15);
    queue.pop_front();
    queue.push_back(1337);
    queue[1] = 666;

    int r{0};
    for(auto it = std::begin(queue); it != std::end(queue); ++it) {
        r += *it;
    }

    return r;

} 

int main() {
    static_assert(std::is_trivially_destructible_v<haz::QueueBase<int, 2>>);
    static_assert(!std::is_trivially_destructible_v<haz::QueueBase<Observer, 2>>);

    static_assert(foo() == 15*2 + 666 + 1337);
}

