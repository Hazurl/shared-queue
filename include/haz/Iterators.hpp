#pragma once

#include <iterator>
#include <new>

namespace haz {

template<typename T, typename C, std::size_t S, bool forward>
class Iterator {
public:

    /* Traits */

    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using const_pointer = T const*;
    using reference = T&;
    using const_reference = T const&;
    using iterator_category = std::random_access_iterator_tag;
    using size_type = std::size_t;
    using this_t = Iterator<T, C, S, forward>;
    using reference_this_t = this_t&;
    using const_reference_this_t = this_t const&;

    using container_type = C;


    /* Constructor */

    constexpr Iterator(container_type data, size_type index) noexcept : _data(data), _index(index) {}
    constexpr Iterator() noexcept : _data(nullptr), _index(0) {}


    /* Iterator requirement */

    constexpr reference operator*() const noexcept {
        size_type wrapped = _index >= S ? _index - S : _index;
        if constexpr (forward) {
            return get(wrapped);
        } else {
            return get(S - 1 - wrapped);
        }
    }


    constexpr reference_this_t operator++() noexcept {
        ++_index;
        return *this;
    }

    /* OutputIterator requirement */

    constexpr this_t operator++(int) noexcept {
        this_t last{ *this };
        ++_index;
        return last;
    }


    /* InputIterator requirement */

    constexpr bool operator==(const_reference_this_t other) const noexcept {
        return (_index == other._index || _data == nullptr) && _data == other._data;
    }


    constexpr bool operator!=(const_reference_this_t other) const noexcept {
        return !(*this == other);
    }


    constexpr pointer operator->() noexcept {
        return &**this;
    }
    constexpr const_pointer operator->() const noexcept {
        return &**this;
    }


    /* BidirectionalIterator requirement */

    constexpr reference_this_t operator--() noexcept {
        --_index;
        return *this;
    }


    constexpr this_t operator--(int) noexcept {
        this_t last{ *this };
        --_index;
        return last;
    }


    /* BidirectionalIterator requirement */

    constexpr reference_this_t operator+=(difference_type n) noexcept {
        _index += n;
        return *this;
    }


    constexpr reference_this_t operator-=(difference_type n) noexcept {
        _index -= n;
        return *this;
    }


    constexpr this_t operator+(difference_type n) const noexcept {
        this_t ret{ *this };
        return ret += n;
    }


    constexpr this_t operator-(difference_type n) const noexcept {
        this_t ret{ *this };
        return ret -= n;
    }


    constexpr difference_type operator-(const_reference_this_t other) const noexcept {
        return _index - other._index;        
    }


    constexpr reference operator[](difference_type n) const noexcept {
        return *(*this + n);
    }


    constexpr bool operator<(const_reference_this_t other) const noexcept {
        return _index < other._index;
    }


    constexpr bool operator>(const_reference_this_t other) const noexcept {
        return _index > other._index;
    }


    constexpr bool operator<=(const_reference_this_t other) const noexcept {
        return _index <= other._index;
    }


    constexpr bool operator>=(const_reference_this_t other) const noexcept {
        return _index >= other._index;
    }


private:

    constexpr reference get(std::size_t const idx) const noexcept {
        return const_cast<reference>(_data[idx]._element);
    }

    container_type _data;
    size_type _index;

};

}