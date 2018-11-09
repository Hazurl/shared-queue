#pragma once

#include <array>
#include <cstddef>

#include <haz/Iterators.hpp>
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

    using iterator = Iterator<T, S, true>;
    using const_iterator = Iterator<const T, S, true>;
    using reverse_iterator = Iterator<T, S, false>;
    using const_reverse_iterator = Iterator<const T, S, false>;

    using this_t = Queue<T, S>;
    using reference_this_t = this_t&;
    using const_reference_this_t = this_t const&;


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
        return _data[_front].value;
    }
    constexpr reference  front() noexcept {
        return _data[_front].value;
    }


    constexpr const_reference back() const noexcept {
        return _data[(_back + S - 1) % S].value;
    }
    constexpr reference back() noexcept {
        return _data[(_back + S - 1) % S].value;
    }


    constexpr const_reference at(size_type index) const {
        if (index >= size()) {
            throw std::out_of_range("Index out of range in SharedQueue::at");
        }

        return _data[(_front + index) % S].value;
    }
    constexpr reference at(size_type index) {
        if (index >= size()) {
            throw std::out_of_range("Index out of range in SharedQueue::at");
        }

        return _data[(_front + index) % S].value;
    }


    constexpr const_reference operator[](size_type index) const noexcept {
        return _data[(_front + index) % S].value;
    }
    constexpr reference operator[](size_type index) noexcept {
        return _data[(_front + index) % S].value;
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


    constexpr void push_back(T const& value) noexcept(noexcept(T(value))) {
        _data[_back].construct(value);
        _back = (_back + 1) % max_size();
        ++_size;
    }
    constexpr void push_back(T&& value) noexcept(noexcept(T(std::move(value)))) {
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
        _data[_front].destruct();
        _front = (_front + 1) % max_size();
    }


    constexpr void swap(reference_this_t other) noexcept {
        std::swap(_front, other._front);
        std::swap(_back, other._back);
        std::swap(_size, other._size);
        std::swap(_data, other._data);
    }


    /* Iterators */

    constexpr iterator begin() noexcept {
        std::cout << "# new " << _front << '\n';
        return iterator(_data.data(), _front);
    }
    constexpr const_iterator begin() const noexcept {
        return const_iterator(_data.data(), _front);
    }
    constexpr const_iterator cbegin() const noexcept {
        return begin();
    }
    

    constexpr iterator end() noexcept {
        std::cout << "# new end " << _front + _size << '\n';
        return iterator(_data.data(), _front + _size);
    }
    constexpr const_iterator end() const noexcept {
        return const_iterator(_data.data(), _front + _size);
    }
    constexpr const_iterator cend() const noexcept {
        return end();
    }

    constexpr reverse_iterator rbegin() noexcept {
        return reverse_iterator(_data.data(), (_front + _size) % max_size());
    }
    constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(_data.data(), (_front + max_size() - 1) % max_size());
    }
    constexpr const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }
    

    constexpr reverse_iterator rend() noexcept {
        return reverse_iterator(_data.data(), (_front + _size) % max_size());
    }
    constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(_data.data(), (_front + max_size() - 1) % max_size());
    }
    constexpr const_reverse_iterator crend() const noexcept {
        return rend();
    }


    /* Comparaison */

    constexpr bool operator==(const_reference_this_t other) const {
        // only constexpr in c++20
        // return std::equal(begin(), end(), other.begin(), other.end());

        if (other.size() != size()) {
            return false;
        }

        auto it = begin();
        for(auto const& elem : other) {
            if (elem != *it++) {
                return false;
            }
        }

        return true;
    }


    constexpr bool operator!=(const_reference_this_t other) const {
        return !(*this == other);
    }


    constexpr bool operator>(const_reference_this_t other) const {
        if (other.size() != size()) {
            return false;
        }

        auto it = begin();
        for(auto const& elem : other) {
            if (*it++ <= elem) {
                return false;
            }
        }

        return true;
    }


    constexpr bool operator>=(const_reference_this_t other) const {
        return !(other > *this);
    }


    constexpr bool operator<(const_reference_this_t other) const {
        return other > *this;
    }


    constexpr bool operator<=(const_reference_this_t other) const {
        return !(*this > other);
    }


private:

    std::array<Manual<T>, S> _data;
    std::size_t _front{0};
    std::size_t _back{0};
    std::size_t _size{0};

};

}