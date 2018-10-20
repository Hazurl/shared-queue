#include <queue>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <random>
#include <chrono>
#include <condition_variable>
#include <atomic>

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

static std::mutex queue_mutex{};
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

void producer(std::queue<data_t>& instr) {
    std::array<data_t, buffer_size> buffer;
    std::size_t size = 0;

    while(true) {
        // Production until buffer is full
        while(size < buffer_size) {
            if (stop.load())
                return;
            
            int n = real_producer();
            scout << "Produce " << n << ", " << size + 1<< "/" << buffer_size << " elements in buffer\n";
            buffer[size++] = n;
        }

        // Move the buffer to the shared queue
        std::unique_lock<std::mutex> lock{queue_mutex};

        while (instr.size() >= size_max)
            can_produce.wait(lock);

        if (stop.load())
            return;

        while(size > 0 && instr.size() < size_max) {
            --size;
            scout << "Insert  " << buffer[size] << ", " << instr.size() + 1 << "/" << size_max << " elements in queue\n";
            instr.push(std::move(buffer[size]));
        }
        lock.unlock();

        can_comsume.notify_one();
    }
}

void consumer(std::queue<data_t>& instr) {
    std::array<data_t, buffer_size> buffer;
    std::size_t size = 0;
    while(true) {
        // Fill the buffer from the shared queue
        std::unique_lock<std::mutex> lock{queue_mutex};

        while (instr.empty())
            can_comsume.wait(lock);

        if (stop.load())
            return;

        while(size < buffer_size && !instr.empty()) {
            scout << "Pop     " << instr.front() << ", " << instr.size() - 1 << "/" << size_max << " elements in queue\n";
            buffer[size++] = std::move(instr.front());
            instr.pop();
        }
        lock.unlock();

        // Comsume until buffer is empty
        while(size > 0) {
            if (stop.load())
                return;
            
            data_t value = buffer[--size];
            real_consumer(value);
            scout << "Comsume " << value << ", " << size << "/" << buffer_size << " elements in buffer\n";
        }

        can_produce.notify_one();
    }
}

int main() {

    std::queue<data_t> data_ts;

    std::thread producer_thread(producer, std::ref(data_ts));
    std::thread consumer_thread(consumer, std::ref(data_ts));

    std::cin.get();
    scout << "Stopping...\n";

    stop.store(true);
    can_comsume.notify_all();
    can_produce.notify_all();

    producer_thread.join();
    consumer_thread.join();
}