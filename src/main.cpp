#include <queue>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <random>
#include <chrono>
#include <condition_variable>
#include <atomic>

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

using Instruction = int;

static std::mutex instr_mutex{};
static std::atomic_bool stop{false};
static std::size_t size_max = 32;

void decode(std::queue<Instruction>& instr) {
    while(!stop.load()) {
        {
            std::lock_guard<std::mutex> lock{instr_mutex};
            if (instr.size() < size_max) {
                int n = std::rand() % 128;
                Out{} << "Adding " << n << "...\n";
                instr.push(n);
                std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000 + 1000));
            }
        }
    }
}

void interpret(std::queue<Instruction>& instr) {
    while(!stop.load()) {
        {
            std::lock_guard<std::mutex> lock{instr_mutex};
            if (!instr.empty()) {
                Out{} << "Interpreting " << instr.front() << "...\n";
                instr.pop();
                std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000 + 500));
            }
        }
    }
}

int main() {

    std::queue<Instruction> instructions;

    std::thread decoder(decode, std::ref(instructions));
    std::thread interpreter(interpret, std::ref(instructions));

    std::cin.get();
    Out{} << "Stopping...\n";
    stop.store(true);

    decoder.join();
    interpreter.join();
}