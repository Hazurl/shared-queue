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

template<typename T>
struct alignas(T) Manual {

    union {
        T value;
    };

    Manual() {};
    ~Manual() {};

    template<typename...Args>
    T& construct(Args&&... args) {
        return *new (&value) T (std::forward<Args>(args)...);
    }

    void destruct() {
        value.~T();
    }
};

class BufferedOut {
public:

    BufferedOut() { out.construct(); }
    ~BufferedOut() { out.destruct(); }

    void dump() { out.destruct(); out.construct(); }

    Out& get() { return out.value; }
    Out const& get() const { return out.value; }

private:

    Manual<Out> out;

};

}