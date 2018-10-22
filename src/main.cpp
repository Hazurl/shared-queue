#include <queue>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <random>
#include <chrono>
#include <condition_variable>
#include <atomic>

#include <haz/SharedQueue.hpp>
#include <haz/Producer.hpp>
#include <haz/Consumer.hpp>
#include <haz/Out.hpp>


using data_t = int;

static std::atomic_bool stop{false};
static constexpr std::size_t size_max = 10;
static constexpr std::size_t buffer_size = 5;

static std::condition_variable can_comsume;
static std::condition_variable can_produce;

data_t real_producer() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int n = std::rand() % 900 + 100;
    return n;
}

void real_consumer(data_t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void producer(haz::Producer<data_t, size_max> prod) {
    while(!stop.load()) {
        data_t value = real_producer();
        scout << "Produce " << value << '\n';

        prod.push(value);
    }
}

void consumer(haz::Consumer<data_t, size_max> cons) {
    while(!stop.load()) {
        data_t value = cons.pop();
        real_consumer(value);
        scout << "Consume " << value << '\n';
    }
}

int main() {
    haz::SharedQueue<data_t, size_max> queue;

    std::thread producer_thread(producer, haz::Producer{queue});
    std::thread consumer_thread(consumer, haz::Consumer{queue});

    std::cin.get();
    scout << "Stopping...\n";

    stop.store(true);

    producer_thread.join();
    consumer_thread.join();
}