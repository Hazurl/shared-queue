#pragma once

#include <iostream>
#include <sstream>
#include <mutex>
#include <variant>

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

    inline constexpr Manual() {};
    inline constexpr Manual(Manual<T> const&) {};
    inline constexpr Manual(Manual<T>&&) {};
    inline ~Manual() {};
    inline constexpr Manual<T>& operator=(Manual<T> const&) { return *this; };
    inline constexpr Manual<T>& operator=(Manual<T>&&) { return *this; };

    template<typename...Args>
    inline constexpr T& construct(Args&&... args) {
        return *new (&value) T (std::forward<Args>(args)...);
    }

    inline constexpr void destruct() {
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