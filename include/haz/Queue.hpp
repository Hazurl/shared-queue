#pragma once

#include <array>
#include <cstddef>

#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S>
class Queue {
public:

    /* Traits */
    using container_type = std::array<Manual<T>, S>;
    using size_type = std::size_t;

    using value_type = T;
    using reference = value_type&;
    using const_reference = reference const;
    using pointer = T*;
    using const_pointer = T const*;

    using iterator = void;
    using const_iterator = void;
    using reverse_iterator = void;
    using const_reverse_iterator = void;



    /* Capacity */

    constexpr size_type capacity() const noexcept {
        return S;
    }


    constexpr size_type max_size() const noexcept {
        return S;
    }


    constexpr size_type size() const noexcept {
        return _size;
    }


    constexpr bool empty() const noexcept {
        return size() == 0;
    }


    constexpr bool full() const noexcept {
        return size() == max_size();
    }



    /* Element Access */

    constexpr const_reference front() const noexcept {
        return _data[_front].get();
    }
    constexpr reference  front() noexcept {
        return _data[_front].get();
    }


    constexpr const_reference back() const noexcept {
        return _data[_back].get();
    }
    constexpr reference back() noexcept {
        return _data[_back].get();
    }


    constexpr const_reference at(size_type index) const {
        if (index >= size()) {
            throw std::out_of_range("Index out of range in SharedQueue::at");
        }

        return _data[(_front + index) % S].get();
    }
    constexpr reference at(size_type index) {
        if (index >= size()) {
            throw std::out_of_range("Index out of range in SharedQueue::at");
        }

        return _data[(_front + index) % S].get();
    }


    constexpr const_reference operator[](size_type index) const noexcept {
        return _data[(_front + index) % S].get();
    }
    constexpr reference operator[](size_type index) noexcept {
        return _data[(_front + index) % S].get();
    }


    /* Modifier */

    constexpr void clear() noexcept {
        if constexpr (std::is_trivially_destructible_v<T>) {
            _size = 0;
            _back = _front;
        } else {
            if (_back < _front || (_back == _front && _size == max_size())) {
                // [.. _back .. _front .. ]

                // 0 -> _back
                for(size_type pos{0}; pos < _back; ++pos) {
                    _data[pos].destruct();
                }
                // _front -> S
                for(size_type pos{_front}; pos < max_size(); ++pos) {
                    _data[pos].destruct();
                }
            } else {
                // [ .. _front .. _back ..]
                for(size_type pos{_front}; pos < _back; ++pos) {
                    _data[pos].destruct();
                }
            }
        }
    }


    constexpr void push(T const& value) noexcept(noexcept(T(value))) {
        _data[_back].construct(value);
        _back = (_back + 1) % max_size();
        ++_size;
    }
    constexpr void push(T&& value) noexcept(noexcept(T(std::move(value)))) {
        _data[_back].construct(std::move(value));
        _back = (_back + 1) % max_size();
        ++_size;
    }


    template<typename...Args>
    constexpr reference emplace_back(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        auto& ref = _data[_back].construct(std::forward<Args>(args)...);
        _back = (_back + 1) % max_size();
        ++_size;
        return ref;
    }


    constexpr void pop_front() noexcept {
        --_size;
        _data[_back].destruct();
        _back = (_back + max_size() - 1) % max_size();
    }


    constexpr void swap(SharedQueue<T, S>& other) noexcept {
        std::swap(_front, other._front);
        std::swap(_back, other._back);
        std::swap(_size, other._size);
        std::swap(_data, other._data);
    }


    T&       top()       { return data[first]; };
    T const& top() const { return data[first]; };

    void pop() {
        --cur_size;
        first = (first + 1) % S;
        not_full_cv.notify_one();
    }

    void push(T const& t) {
        if (cur_size >= S)
            return;

        ++cur_size;
        data[last] = t;
        last = (last + 1) % S;
        not_empty_cv.notify_one();
    }

    std::size_t size() const {
        return cur_size;
    }

    bool empty() const {
        return cur_size == 0;
    }

    constexpr std::size_t capacity() const {
        return S;
    }

private:

    std::array<Manual<T>, S> data;
    std::size_t first{0};
    std::size_t last{0};
    std::size_t cur_size{0};

};

}