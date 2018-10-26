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
#include <haz/Producer.hpp>
#include <haz/Consumer.hpp>
#include <haz/Out.hpp>
#include <haz/Policy.hpp>


using data_t = int;

static std::atomic_bool stop{false};
static std::atomic_uint count{0};
static constexpr std::size_t size_max = 5;

static std::condition_variable can_comsume;
static std::condition_variable can_produce;

using Prod_t = haz::Producer<data_t, size_max, haz::Buffered<15>>;
using Cons_t = haz::Consumer<data_t, size_max, haz::Buffered<10>>;

data_t real_producer() {
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int n = std::rand() % 900 + 100;
    return n;
}

void real_consumer(data_t d, std::ostream& os) {
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    os << d << '\n';
    ++count;
}

void producer(Prod_t prod) {
    try {
        while(!stop.load()) {
            data_t value = real_producer();
            //scout << "Produce " << value << '\n';

            prod.push(value);
        }
    } catch(...) {}
}

void consumer(Cons_t cons) {
    haz::BufferedOut out;
    std::size_t out_count = 0;
    static constexpr std::size_t max = 100;
    try {
        while(!stop.load()) {
            data_t value = cons.pop();
            real_consumer(value, out.get());
            //scout << "Consume " << value << '\n';

            if (++out_count >= max) {
                out.dump(); 
                out_count = 0;
            }
        }
    } catch(...) {}
}

int main() {
    haz::SharedQueue<data_t, size_max> queue;

    auto start = std::chrono::system_clock::now();
    std::thread producer_thread(producer, Prod_t{queue});
    std::thread consumer_thread1(consumer, Cons_t{queue});/*
    std::thread consumer_thread2(consumer, Cons_t{queue});
    std::thread consumer_thread3(consumer, Cons_t{queue});
    std::thread consumer_thread4(consumer, Cons_t{queue});*/

    std::cin.get();
    scout << "Stopping...\n";

    stop.store(true);
    queue.interrupt_all();

    producer_thread.join();
    consumer_thread1.join();/*
    consumer_thread2.join();
    consumer_thread3.join();
    consumer_thread4.join();*/
    auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed = end - start;
    scout << "Elapsed: " << count.load()  << " in " << elapsed.count() << "s ~" << static_cast<double>(count.load()) / elapsed.count() << " data/s\n"; 
}