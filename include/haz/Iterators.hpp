#pragma once

#include <haz/Out.hpp>

namespace haz {

template<typename T, std::size_t S>
class Iterator {
public:

    /* Traits */

    using difference_type = std::ptrdiff_t;
    using valut_type = T;
    using pointer = T*;
    using const_pointer = T const*;
    using reference = T&;
    using const_reference = reference const;
    using iterator_category = std::random_access_iterator_tag;
    using size_type = std::size_t;
    using this_t = Iterator<T, S>;
    using reference_this_t = this_t&;
    using const_reference_this_t = reference_this_t const;


    /* Iterator requirement */

    constexpr reference operator*() noexcept {
        return _data[index];
    }
    constexpr const_reference operator*() const noexcept {
        return _data[index];
    }


    constexpr reference_this_t operator++() noexcept {
        index = (index + 1) % S;
        return *this;
    }

    /* OutputIterator requirement */

    constexpr this_t operator++(int) noexcept {
        this_t last{ *this };
        index = (index + 1) % S;
        return last;
    }


    /* InputIterator requirement */

    constexpr bool operator==(const_reference_this_t other) const noexcept {
        return index == other.index && _data == other._data;
    }


    constexpr bool operator!=(const_reference_this_t other) const noexcept {
        return !(*this == *other);
    }


    constexpr pointer operator->() noexcept {
        return &**this;
    }
    constexpr const_pointer operator->() const noexcept {
        return &**this;
    }


    /* BidirectionalIterator requirement */

    constexpr reference_this_t operator--() noexcept {
        index = (index + S - 1) % S;
        return *this;
    }


    constexpr this_t operator--(int) noexcept {
        this_t last{ *this };
        index = (index + S - 1) % S;
        return last;
    }


    /* BidirectionalIterator requirement */

    constexpr reference_this_t operator+=(difference_type n) noexcept {
        index = (index + S + n) % S;
        return *this;
    }


    constexpr reference_this_t operator-=(difference_type n) noexcept {
        index = (index + S - n) % S;
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
        
    }


private:

    T* _data;
    size_type index;
    size_type _front;
    size_type _size;

};

}