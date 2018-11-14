#pragma once

#include <array>
#include <cstddef>
#include <cassert>
#include <memory>
#include <new>

#include <haz/Iterators.hpp>
#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S>
class Queue {
public:

    /* Traits */
    using manual_t = struct alignas(T) { std::byte byte[sizeof(T)]; };
    using container_type = std::array<manual_t, S>;
    using size_type = std::size_t;

    using value_type = T;
    using reference = value_type&;
    using const_reference = reference const;
    using pointer = T*;
    using const_pointer = T const*;

    using iterator = Iterator<T, manual_t*, S, true>;
    using const_iterator = Iterator<const T, manual_t const*, S, true>;
    using reverse_iterator = Iterator<T, manual_t*, S, false>;
    using const_reverse_iterator = Iterator<const T, manual_t const*, S, false>;

    using this_t = Queue<T, S>;
    using reference_this_t = this_t&;
    using const_reference_this_t = this_t const&;


    /* Constructors */

    constexpr Queue() : _data{}, _front{0}, _back{_front}, _size{0} {
        std::cerr << "\t|[Default Constructor]|\n";
    }

    constexpr Queue(size_type n) : _front{0}, _back{n}, _size{n} {
        assert(n <= max_size());
        if constexpr (!std::is_trivially_default_constructible_v<T>) {
            std::uninitialized_default_construct_n(get(0), n);
        }
        std::cerr << "\t|[Copy-1 Constructor]|\n";
    }

    constexpr Queue(size_type n, T const& value) : _front{0}, _back{n}, _size{n} {
        assert(n <= max_size());
        for(size_type i{0}; i < n; ++i) {
            std::uninitialized_default_construct_n(get(0), n);
            construct(i, value);
        }
        std::cerr << "\t|[Copy-n Constructor]|\n";
    }

    template<typename It>
    constexpr Queue(It first, It last) : Queue() {
        for(; first != last; ++first) {
            construct(_back, *first);
            ++_back;
            ++_size;
        }
        std::cerr << "\t|[Iterator Constructor]|\n";
    }

    template<typename R>
    constexpr Queue(std::initializer_list<R> list) : Queue() {
        std::cerr << "\t|[BEGIN Initializer List Constructor]|\n";
        std::uninitialized_copy(std::begin(list), std::end(list), get(0));
        _back = _size = list.size();
        std::cerr << "\t|[Initializer List Constructor]|\n";
    }


    /* Specials Members */


    ~Queue() {
        clear();
    }


    constexpr Queue(const_reference_this_t other) : _front{other._front}, _back{other._back}, _size{other._size} {
        std::uninitialized_copy(std::begin(other), std::end(other), get(0));
        std::cerr << "\t|[Copy Constructor]|\n";
    }

    constexpr Queue(this_t&& other) : _front{other._front}, _back{other._back}, _size{other._size} {
        std::cerr << "\t|[BEGIN Move Constructor]|\n";
        std::uninitialized_move(std::begin(other), std::end(other), get(0));
        std::cerr << "\t|[Move Constructor]|\n";
    }

    constexpr reference_this_t operator=(const_reference_this_t other) {
        if (&other == this) return *this;

        clear();
        std::uninitialized_copy(std::begin(other), std::end(other), get(0));

        _front = 0;
        _back = _size = other._size;

        std::cerr << "\t|[Copy operator=]|\n";
        return *this;
    }

    constexpr reference_this_t operator=(this_t&& other) {
        if (&other == this) return *this;

        clear();
        std::uninitialized_move(std::begin(other), std::end(other), get(0));

        _front = 0;
        _back = _size = other._size;

        std::cerr << "\t|[Move operator=]|\n";
        return *this;
    }

    constexpr reference_this_t operator=(std::initializer_list<T> list) {
        clear();
        for(auto&& elem : list) {
            emplace_back(std::move(elem));
        }
    }

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
        return *get(_front);
    }
    constexpr reference  front() noexcept {
        return *get(_front);
    }


    constexpr const_reference back() const noexcept {
        return *get((_back + S - 1) % S);
    }
    constexpr reference back() noexcept {
        return *get((_back + S - 1) % S);
    }


    constexpr const_reference at(size_type index) const {
        if (index >= size()) {
            throw std::out_of_range("Index out of range in SharedQueue::at");
        }

        return *get((_front + index) % S);
    }
    constexpr reference at(size_type index) {
        if (index >= size()) {
            throw std::out_of_range("Index out of range in SharedQueue::at");
        }

        return *get((_front + index) % S);
    }


    constexpr const_reference operator[](size_type index) const noexcept {
        return *get((_front + index) % S);
    }
    constexpr reference operator[](size_type index) noexcept {
        return *get((_front + index) % S);
    }


    /* Modifier */

    constexpr void clear() noexcept {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            if (_back < _front || (_back == _front && _size == max_size())) {
                // [.. _back .. _front .. ]

                // 0 -> _back
                std::destroy_n(get(0), _back);
                // _front -> S
                std::destroy_n(get(_front), max_size() - _front);
            } else {
                // [ .. _front .. _back ..]
                std::destroy_n(get(_front), _back - _front);
            }
        }
        _back = _front = _size = 0;
    }


    constexpr void push_back(T const& value) noexcept(noexcept(T(value))) {
        construct(_back, value);
        _back = (_back + 1) % max_size();
        ++_size;
    }
    constexpr void push_back(T&& value) noexcept(noexcept(T(std::move(value)))) {
        construct(_back, std::move(value));
        _back = (_back + 1) % max_size();
        ++_size;
    }


    template<typename...Args>
    constexpr reference emplace_back(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        auto& ref = construct(_back, std::forward<Args>(args)...);
        _back = (_back + 1) % max_size();
        ++_size;
        return ref;
    }


    constexpr void pop_front() noexcept {
        --_size;
        destruct(_front);
        _front = (_front + 1) % max_size();
    }


    constexpr void swap(reference_this_t other) 
        noexcept(
            noexcept(std::is_nothrow_swappable_v<decltype(other._front)>) && 
            noexcept(std::is_nothrow_swappable_v<decltype(other._back)>) &&
            noexcept(std::is_nothrow_swappable_v<decltype(other._size)>) && 
            noexcept(std::is_nothrow_swappable_v<decltype(other._data)>)) {
        std::swap(_front, other._front);
        std::swap(_back, other._back);
        std::swap(_size, other._size);
        std::swap(_data, other._data);
    }


    /* Iterators */

    constexpr iterator begin() noexcept {
        return iterator(_data.data(), _front);
    }
    constexpr const_iterator begin() const noexcept {
        return const_iterator(_data.data(), _front);
    }
    constexpr const_iterator cbegin() const noexcept {
        return begin();
    }
    

    constexpr iterator end() noexcept {
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

    constexpr bool operator==(const_reference_this_t other) const noexcept(noexcept(std::declval<T>() != std::declval<T>())) {
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


    constexpr bool operator!=(const_reference_this_t other) const noexcept(noexcept(std::declval<const_reference_this_t>() == std::declval<const_reference_this_t>())) {
        return !(*this == other);
    }


    constexpr bool operator<(const_reference_this_t other) const noexcept(noexcept(std::declval<T>() < std::declval<T>()) && noexcept(std::declval<T>() > std::declval<T>())) {
        auto first1 = begin();
        auto first2 = other.begin();

        auto const last1 = end();
        auto const last2 = other.end();

        for(; (first1 != last1) && (first2 != last2); (++first1), (++first2)) {
            if (*first1 < *first2) {
                return true;
            } else if (*first1 > *first2) {
                return false;
            }
        }

        return (first1 == last1) && (first2 != last2);
    }


    constexpr bool operator<=(const_reference_this_t other) const noexcept(noexcept(std::declval<const_reference_this_t>() < std::declval<const_reference_this_t>())) {
        return !(other < *this);
    }


    constexpr bool operator>(const_reference_this_t other) const noexcept(noexcept(std::declval<const_reference_this_t>() < std::declval<const_reference_this_t>())) {
        return other < *this;
    }


    constexpr bool operator>=(const_reference_this_t other) const noexcept(noexcept(std::declval<const_reference_this_t>() < std::declval<const_reference_this_t>())) {
        return !(*this < other);
    }


private:

    template<typename...Args>
    constexpr reference construct(std::size_t const idx, Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        return *new(get(idx)) T (std::forward<Args>(args)...);
    }

    constexpr void destruct(std::size_t const idx) noexcept {
        std::destroy_at(get(idx));
    }

    constexpr pointer get(std::size_t const idx) noexcept {
        return std::launder(reinterpret_cast<pointer>(std::addressof(_data[idx])));
    }

    constexpr const_pointer get(std::size_t const idx) const noexcept {
        return std::launder(reinterpret_cast<const_pointer>(std::addressof(_data[idx])));
    }


    container_type _data;
    std::size_t _front{0};
    std::size_t _back{0};
    std::size_t _size{0};

};

}

namespace std {

template<typename T, std::size_t S>
constexpr void swap(haz::Queue<T, S>& lhs, haz::Queue<T, S>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    return lhs.swap(rhs);
}

}