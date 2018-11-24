#pragma once

#include <array>
#include <cstddef>
#include <cassert>
#include <memory>
#include <new>

#include <haz/Iterators.hpp>
#include <haz/Out.hpp>

namespace haz {

struct Checker {
    template<typename...Args>
    static constexpr void are_arguments_constexpr(Args&&...) {}
};

template<typename...Args>
static constexpr void are_arguments_constexpr(Args&&...) {}

template<int>
using Void = void;

template<typename F>
constexpr
auto
is_a_constant_expression(F&& f)
    -> decltype( ( std::forward<F>(f)() , std::true_type{} ) )
{ return {}; }
constexpr
std::false_type is_a_constant_expression(...)
{ return {}; }

#define is_constexpr(...) \
is_a_constant_expression([]()-> Void<(are_arguments_constexpr(__VA_ARGS__), 0)>{})





struct QueueElementConstructElement {};

template<typename T>
union CommonQueueElement {
    char _placeholder;
    T _element;

    template<typename...Args>
    inline T& construct(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        return *new(std::addressof(_element)) T (std::forward<Args>(args)...);
    }

    inline void destruct() noexcept {
        std::destroy_at(std::addressof(_element));
    }


    CommonQueueElement() noexcept : _placeholder{} {}
    template<typename...Args>
    CommonQueueElement(QueueElementConstructElement, Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) : _element(std::forward<Args>(args)...) {}

    ~CommonQueueElement() {}

};

template<typename T>
union ConstexprQueueElement {
    char _placeholder;
    T _element;

    template<typename...Args>
    constexpr inline T& construct(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        if constexpr(is_constexpr(std::forward<Args>(args)...)) {
            T tmp(std::forward<Args>(args)...);
            return _element = std::move(tmp);
        } else {
            return *new(std::addressof(_element)) T (std::forward<Args>(args)...);
        }
    }

    // trivial destructor
    constexpr inline void destruct() noexcept {}

    constexpr ConstexprQueueElement() noexcept(T()) : _element() {}
    template<typename...Args>
    constexpr ConstexprQueueElement(QueueElementConstructElement, Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) : _element(std::forward<Args>(args)...) {}

    ~ConstexprQueueElement() = default;
};

template<typename T, std::size_t S>
class CommonQueueBase {
public:

    using value_type             = T;
    using const_value_type       = T const;
    using reference              = value_type&;
    using const_reference        = reference const;
    using pointer                = value_type*;
    using const_pointer          = value_type const*;

    using contained_type         = CommonQueueElement<T>;
    using container_type         = std::array<contained_type, S>;
    using size_type              = std::size_t;

    using iterator               = Iterator<value_type,         contained_type*,        S, true>;
    using const_iterator         = Iterator<const_value_type,   contained_type const*,  S, true>;
    using reverse_iterator       = Iterator<value_type,         contained_type*,        S, false>;
    using const_reverse_iterator = Iterator<const_value_type,   contained_type const*,  S, false>;

    template<typename D>
    constexpr inline CommonQueueBase(D&& d, size_type front, size_type back, size_type size) : _data(std::forward<D>(d)), _front{front}, _back{back}, _size{size} {}

    constexpr inline CommonQueueBase() = default;

    ~CommonQueueBase() {
        _destructor();
    }

    void clear() noexcept {
        _destructor();
        _front = _back = _size = 0;
    }

protected:


    inline constexpr pointer get(std::size_t const idx) noexcept {
        return std::launder(reinterpret_cast<pointer>(_data.data() + idx));
    }

    inline constexpr const_pointer get(std::size_t const idx) const noexcept {
        return std::launder(reinterpret_cast<const_pointer>(_data.data() + idx));
    }


    template<typename...Args>
    inline constexpr reference construct(std::size_t const idx, Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        return _data[idx].construct(std::forward<Args>(args)...);
    }

    inline constexpr void destruct(std::size_t const idx) noexcept {
        return _data[idx].destruct();
    }


    container_type _data{};
    size_type _front{0};
    size_type _back{0};
    size_type _size{0};

private:

    void _destructor() noexcept {
        if (_back < _front || (_back == _front && _size == S)) {
            // [.. _back .. _front .. ]

            // 0 -> _back
            std::destroy_n(get(0), _back);
            // _front -> S
            std::destroy_n(get(_front), S - _front);
        } else {
            // [ .. _front .. _back ..]
            std::destroy_n(get(_front), _back - _front);
        }
    }

};

template<typename T, std::size_t S>
class ConstexprQueueBase {
public:

    using value_type             = T;
    using const_value_type       = T const;
    using reference              = value_type&;
    using const_reference        = reference const;
    using pointer                = value_type*;
    using const_pointer          = value_type const*;

    using contained_type         = ConstexprQueueElement<T>;
    using container_type         = std::array<ConstexprQueueElement<T>, S>;
    using size_type              = std::size_t;

    using iterator               = Iterator<value_type,         contained_type*,        S, true>;
    using const_iterator         = Iterator<const_value_type,   contained_type const*,  S, true>;
    using reverse_iterator       = Iterator<value_type,         contained_type*,        S, false>;
    using const_reverse_iterator = Iterator<const_value_type,   contained_type const*,  S, false>;

    template<typename D>
    constexpr inline ConstexprQueueBase(D&& d, size_type front, size_type back, size_type size) : _data(std::forward<D>(d)), _front{front}, _back{back}, _size{size} {}
    constexpr inline ConstexprQueueBase(size_type front, size_type back, size_type size) : _data{}, _front{front}, _back{back}, _size{size} {}

    constexpr inline ConstexprQueueBase() = default;

    ~ConstexprQueueBase() = default;

    constexpr void clear() noexcept {
        _front = _back = _size = 0;
    }

protected:


    inline constexpr pointer get(std::size_t const idx) noexcept {
        return std::addressof(_data[idx]._element);
    }

    inline constexpr const_pointer get(std::size_t const idx) const noexcept {
        return std::addressof(_data[idx]._element);
    }


    template<typename...Args>
    inline constexpr reference construct(std::size_t const idx, Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        return _data[idx].construct(std::forward<Args>(args)...);
    }

    inline constexpr void destruct(std::size_t const idx) noexcept {
        return _data[idx].destruct();
    }


    container_type _data{};
    size_type _front{0};
    size_type _back{0};
    size_type _size{0};

};

template<typename T, std::size_t S>
using QueueBase = std::conditional_t<
    std::is_trivially_destructible_v<T>, 
    ConstexprQueueBase<T, S>,
    CommonQueueBase<T, S>>;


template<typename T, std::size_t S>
class Queue : public QueueBase<T, S> {

    using base_t = QueueBase<T, S>;

public:

    /* Traits */
    using typename base_t::value_type;
    using typename base_t::const_value_type;
    using typename base_t::reference;
    using typename base_t::const_reference;
    using typename base_t::pointer;
    using typename base_t::const_pointer;

    using typename base_t::container_type;
    using typename base_t::size_type;

    using typename base_t::iterator;
    using typename base_t::const_iterator;
    using typename base_t::reverse_iterator;
    using typename base_t::const_reverse_iterator;

    using this_t = Queue<T, S>;
    using reference_this_t = this_t&;
    using const_reference_this_t = this_t const&;

private:

    using base_t::get;

public:

    /* Constructors */

    inline constexpr Queue() : base_t(0, 0, 0) {}


    explicit inline constexpr Queue(size_type n) : base_t(0, n, n) {
        assert(n <= S);
        if constexpr (!std::is_trivially_default_constructible_v<T>) {
            std::uninitialized_default_construct_n(get(0), n);
        }
    }


    explicit inline constexpr Queue(size_type n, T const& value) : base_t(0, n, n) {
        assert(n <= S);
        for(size_type i{0}; i < n; ++i) {
            base_t::construct(i, value);
        }
    }


    template<typename It, typename = std::enable_if_t<!std::is_integral_v<It>>>
    constexpr Queue(It first, It last) : Queue() {
        for(; first != last; ++first) {
            base_t::construct(base_t::_back++, *first);
        }

        base_t::_size = base_t::_back;
    }


    template<typename R>
    inline constexpr Queue(std::initializer_list<R> list) : base_t(0, 0, 0) {
        assign(std::begin(list), std::end(list));
    }


    /* Specials Members */

    inline constexpr Queue(const_reference_this_t other) : base_t(0, other.size, other.size) {
        std::uninitialized_copy(std::begin(other), std::end(other), get(0));
    }


    inline constexpr Queue(this_t&& other) : base_t(0, other.size, other.size) {
        std::uninitialized_move(std::begin(other), std::end(other), get(0));
    }


    constexpr reference_this_t operator=(const_reference_this_t other) {
        if (&other == this) return *this;

        clear();
        std::uninitialized_copy(std::begin(other), std::end(other), get(0));

        base_t::_back = base_t::_size = other._size;

        return *this;
    }


    constexpr reference_this_t operator=(this_t&& other) {
        if (&other == this) return *this;

        clear();
        std::uninitialized_move(std::begin(other), std::end(other), get(0));

        base_t::_back = base_t::_size = other._size;

        std::cerr << "\t|[Move operator=]|\n";
        return *this;
    }


    inline constexpr reference_this_t operator=(std::initializer_list<T> list) {
        assign(std::begin(list), std::end(list));
    }


    /* Capacity */

    inline constexpr size_type capacity() const noexcept {
        return S;
    }


    inline constexpr size_type max_size() const noexcept {
        return S;
    }


    inline constexpr size_type size() const noexcept {
        return base_t::_size;
    }


    inline constexpr bool empty() const noexcept {
        return size() == 0;
    }


    inline constexpr bool full() const noexcept {
        return size() == S;
    }



    /* Element Access */

    inline constexpr const_reference front() const noexcept {
        return *get(base_t::_front);
    }
    inline constexpr reference  front() noexcept {
        return *get(base_t::_front);
    }


    inline constexpr const_reference back() const noexcept {
        return *get((base_t::_back + S - 1) % S);
    }
    inline constexpr reference back() noexcept {
        return *get((base_t::_back + S - 1) % S);
    }


    inline constexpr const_reference at(size_type index) const {
        if (index >= size()) {
            throw std::out_of_range("Index out of range in SharedQueue::at");
        }

        return *get((base_t::_front + index) % S);
    }
    inline constexpr reference at(size_type index) {
        if (index >= size()) {
            throw std::out_of_range("Index out of range in SharedQueue::at");
        }

        return *get((base_t::_front + index) % S);
    }


    inline constexpr const_reference operator[](size_type index) const noexcept {
        return *get((base_t::_front + index) % S);
    }
    inline constexpr reference operator[](size_type index) noexcept {
        return *get((base_t::_front + index) % S);
    }


    /* Modifier */

    using base_t::clear;


    template<typename It>
    constexpr void assign(It first, It last) noexcept(std::is_nothrow_copy_constructible_v<T>) {
        clear();
        auto it = std::uninitialized_copy(std::move(first), std::move(last), get(0));

        base_t::_back = base_t::_size = std::distance(get(0), it);
        base_t::_front = 0;
    }


    constexpr void push_back(T const& value) noexcept(noexcept(T(value))) {
        base_t::construct(base_t::_back, value);
        base_t::_back = (base_t::_back + 1) % S;
        ++base_t::_size;
    }
    constexpr void push_back(T&& value) noexcept(noexcept(T(std::move(value)))) {
        base_t::construct(base_t::_back, std::move(value));
        base_t::_back = (base_t::_back + 1) % S;
        ++base_t::_size;
    }


    template<typename...Args>
    constexpr reference emplace_back(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
        auto& ref = base_t::construct(base_t::_back, std::forward<Args>(args)...);
        base_t::_back = (base_t::_back + 1) % S;
        ++base_t::_size;
        return ref;
    }


    constexpr void pop_front() noexcept {
        --base_t::_size;
        base_t::destruct(base_t::_front);
        base_t::_front = (base_t::_front + 1) % S;
    }


    constexpr void swap(reference_this_t other) 
        noexcept(noexcept(std::is_nothrow_swappable_v<decltype(other._data)>)) {
        std::swap(base_t::_front, other._front);
        std::swap(base_t::_back, other._back);
        std::swap(base_t::_size, other._size);
        std::swap(base_t::_data, other._data);
    }


    /* Iterators */

    inline constexpr auto data() {
        return base_t::_data.data();
    }
    inline constexpr auto data() const {
        return base_t::_data.data();
    }

    inline constexpr iterator begin() noexcept {
        return iterator(base_t::_data.data(), base_t::_front);
    }
    inline constexpr const_iterator begin() const noexcept {
        return const_iterator(base_t::_data.data(), base_t::_front);
    }
    inline constexpr const_iterator cbegin() const noexcept {
        return begin();
    }
    

    inline constexpr iterator end() noexcept {
        return iterator(base_t::_data.data(), base_t::_front + base_t::_size);
    }
    inline constexpr const_iterator end() const noexcept {
        return const_iterator(base_t::_data.data(), base_t::_front + base_t::_size);
    }
    inline constexpr const_iterator cend() const noexcept {
        return end();
    }

    inline constexpr reverse_iterator rbegin() noexcept {
        return reverse_iterator(base_t::_data.data(), (base_t::_front + base_t::_size) % S);
    }
    inline constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(base_t::_data.data(), (base_t::_front + S - 1) % S);
    }
    inline constexpr const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }
    

    inline constexpr reverse_iterator rend() noexcept {
        return reverse_iterator(base_t::_data.data(), (base_t::_front + base_t::_size) % S);
    }
    inline constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(base_t::_data.data(), (base_t::_front + S - 1) % S);
    }
    inline constexpr const_reverse_iterator crend() const noexcept {
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


    inline constexpr bool operator!=(const_reference_this_t other) const noexcept(noexcept(std::declval<const_reference_this_t>() == std::declval<const_reference_this_t>())) {
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


    inline constexpr bool operator<=(const_reference_this_t other) const noexcept(noexcept(std::declval<const_reference_this_t>() < std::declval<const_reference_this_t>())) {
        return !(other < *this);
    }


    inline constexpr bool operator>(const_reference_this_t other) const noexcept(noexcept(std::declval<const_reference_this_t>() < std::declval<const_reference_this_t>())) {
        return other < *this;
    }


    inline constexpr bool operator>=(const_reference_this_t other) const noexcept(noexcept(std::declval<const_reference_this_t>() < std::declval<const_reference_this_t>())) {
        return !(*this < other);
    }

};

}

namespace std {

template<typename T, std::size_t S>
constexpr void swap(haz::Queue<T, S>& lhs, haz::Queue<T, S>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    return lhs.swap(rhs);
}

}