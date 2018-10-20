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

#define scout ::Out{}

class Out : public std::ostringstream {
public:
    Out() = default;
    ~Out() {
        std::lock_guard<std::mutex> guard { mutex };
        std::cout << str();
    }
private:
    static std::mutex mutex;
};

std::mutex Out::mutex{};

using data_t = int;

static std::atomic_bool stop{false};
static constexpr std::size_t size_max = 10;
static constexpr std::size_t buffer_size = 5;

static std::condition_variable can_comsume;
static std::condition_variable can_produce;

data_t real_producer() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    int n = std::rand() % 900 + 100;
    return n;
}

void real_consumer(data_t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void producer(haz::SharedQueue<data_t, size_max>& instr) {
    std::array<data_t, buffer_size> buffer;
    std::size_t size = 0;

    while(true) {
        data_t value = real_producer();
        scout << "Produce " << value << '\n';

producer_lock:
        instr.lock();

        if (instr.size() >= instr.capacity()) {
            instr.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds{10});
            goto producer_lock;
        }

        instr.push(value);
        scout << "Insert  " << value << ", " << instr.size() << "/" << instr.capacity() << " elements in queue\n";

        instr.unlock();
        /*
        // Production until buffer is full
        while(size < buffer_size) {
            if (stop.load())
                return;
            
            int n = real_producer();
            scout << "Produce " << n << ", " << size + 1<< "/" << buffer_size << " elements in buffer\n";
            buffer[size++] = n;
        }

        // Move the buffer to the shared queue
        {
            std::lock_guard lock {instr};
            while (instr.size() >= size_max)
                std::this_thread::sleep_for(std::chrono::milliseconds{10});//can_produce.wait(lock);

            if (stop.load())
                return;

            //instr.push_all(std::begin(buffer), std::end(buffer));
            while(size > 0 && instr.size() < size_max) {
                --size;
                scout << "Insert  " << buffer[size] << ", " << instr.size() + 1 << "/" << size_max << " elements in queue\n";
                instr.push(std::move(buffer[size]));
            }
        }

        //can_comsume.notify_one();*/
    }
}

void consumer(haz::SharedQueue<data_t, size_max>& instr) {
    std::array<data_t, buffer_size> buffer;
    std::size_t size = 0;
    while(true) {
        instr.lock();

        if (instr.empty()) {
            instr.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds{10});
            continue;
        }

        data_t value = instr.top();
        instr.pop();
        scout << "Pop     " << value << ", " << instr.size() << "/" << instr.capacity() << " elements in queue\n";

        instr.unlock();

        real_consumer(value);
        scout << "Comsume " << value << '\n';

        // Fill the buffer from the shared queue
        /*{
            while (!instr.try_lock())
                std::this_thread::sleep_for(std::chrono::milliseconds{10});//can_comsume.wait(lock);

            if (instr.empty()) {
                instr.unlock();
                continue;
            }

            if (stop.load())
                return;

            while(size < buffer_size && !instr.empty()) {
                scout << "Pop     " << instr.top() << ", " << instr.size() - 1 << "/" << size_max << " elements in queue\n";
                buffer[size++] = std::move(instr.top());
                instr.pop();
            }
        }

        // Comsume until buffer is empty
        while(size > 0) {
            if (stop.load())
                return;
            
            data_t value = buffer[--size];
            real_consumer(value);
            scout << "Comsume " << value << ", " << size << "/" << buffer_size << " elements in buffer\n";
        }*/

        //can_produce.notify_one();
    }
}

int main() {

    haz::SharedQueue<data_t, size_max> queue;

    std::thread producer_thread(producer, std::ref(queue));
    std::thread consumer_thread(consumer, std::ref(queue));

    std::cin.get();
    scout << "Stopping...\n";

    stop.store(true);
    can_comsume.notify_all();
    can_produce.notify_all();

    producer_thread.join();
    consumer_thread.join();
}