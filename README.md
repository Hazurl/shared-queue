# Multithreaded Queue

Consumer/Producer pattern implementation.
Any number of thread can be either a producer or a consumer, respectively pushing and poping to and from the queue in a transparent way.
An additional policy can be used to add a buffer so that the lock become less stressed.

```cpp 
#include <haz/SharedQueue.hpp>
#include <haz/Producer.hpp>
#include <haz/Consumer.hpp>
#include <thread>
#include <atomic>

using data_t = int;

static std::atomic_bool stop{false};
static constexpr std::size_t size_max = 50;

using Prod_t = haz::Producer<data_t /* Queue's type */, size_max /* Queue's size */, haz::Buffered<15> /* Policy */>;
using Cons_t = haz::Consumer<data_t,                    size_max,                    haz::Buffered<10>>;

data_t real_producer() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // fake work
    data_t n = std::rand() % 900 + 100;
    return n;
}

void real_consumer(data_t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // fake work
}

void producer(Prod_t prod) {
    try {
        while(!stop.load()) {
            data_t value = real_producer();
            prod.push(value);
        }
    } catch(std::runtime_error const&) { /* Interrupt */ } 
}

void consumer(Cons_t cons) {
    try {
        while(!stop.load()) {
            data_t value = cons.pop();
            real_consumer(value);
        }
    } catch(std::runtime_error const&) { /* Interrupt */ } 
}

int main() {
    haz::SharedQueue<data_t, size_max> queue;

    std::thread producer_thread(producer, Prod_t{queue});
    std::thread consumer_thread(consumer, Cons_t{queue});

    std::cin.get();
    scout << "Stopping...\n";

    stop.store(true);      // Stop all non-waiting thread
    queue.interrupt_all(); // Stop all waiting thread, a runtime_error will be threw

    producer_thread.join();
    consumer_thread.join();
}```