#pragma once

#include <iostream>
#include <sstream>
#include <mutex>

#define scout ::haz::Out{}

namespace haz {

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

}