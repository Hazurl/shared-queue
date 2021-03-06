#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <atomic>
#include <experimental/type_traits> // srd::is_detected

#include <haz/Queue.hpp>

namespace std { using namespace experimental; } // don't judge me


std::atomic<unsigned> error_count{0};
std::mutex error_message_mutex;
std::vector<std::string> error_messages;

enum class Restriction {
    Deleted, Trivial, Constexpr, ConstexprTrivial
};

///////////////////

template<Restriction r, bool nex>
struct _DCtr;

template<bool nex>
struct _DCtr<Restriction::ConstexprTrivial, nex> {
    constexpr _DCtr () noexcept(nex) = default;
};

template<bool nex>
struct _DCtr<Restriction::Deleted, nex> {
    _DCtr () noexcept(nex) = delete;
};

template<bool nex>
struct _DCtr<Restriction::Trivial, nex> {
    _DCtr () noexcept(nex) = default;
private: int _;
};

template<bool nex>
struct _DCtr<Restriction::Constexpr, nex> {
    constexpr _DCtr () noexcept(nex) {};
};

///////////////////

template<Restriction r, bool nex>
struct _ICtr;

template<bool nex>
struct _ICtr<Restriction::Deleted, nex> {
    _ICtr (int) noexcept(nex) = delete;
};

template<bool nex>
struct _ICtr<Restriction::Constexpr, nex> {
    constexpr _ICtr (int) noexcept(nex) {};
};

///////////////////

template<Restriction r, bool nex>
struct _Dtr;

template<bool nex>
struct _Dtr<Restriction::ConstexprTrivial, nex> {
    ~_Dtr () noexcept(nex) = default;
};

template<bool nex>
struct _Dtr<Restriction::Deleted, nex> {
    ~_Dtr () noexcept(nex) = delete;
};

///////////////////

template<Restriction r, bool nex>
struct _Cpy_Ctr;

template<bool nex>
struct _Cpy_Ctr<Restriction::ConstexprTrivial, nex> {
    constexpr _Cpy_Ctr (_Cpy_Ctr const&) noexcept(nex) = default;
};

template<bool nex>
struct _Cpy_Ctr<Restriction::Deleted, nex> {
    _Cpy_Ctr (_Cpy_Ctr const&) noexcept(nex) = delete;
};

template<bool nex>
struct _Cpy_Ctr<Restriction::Trivial, nex> {
    _Cpy_Ctr (_Cpy_Ctr const&) noexcept(nex) = default;
};

template<bool nex>
struct _Cpy_Ctr<Restriction::Constexpr, nex> {
    constexpr _Cpy_Ctr (_Cpy_Ctr const&) noexcept(nex) {};
};

///////////////////

template<Restriction r, bool nex>
struct _Mv_Ctr;

template<bool nex>
struct _Mv_Ctr<Restriction::ConstexprTrivial, nex> {
    constexpr _Mv_Ctr (_Mv_Ctr&&) noexcept(nex) = default;
};

template<bool nex>
struct _Mv_Ctr<Restriction::Deleted, nex> {
    _Mv_Ctr (_Mv_Ctr&&) noexcept(nex) = delete;
};

template<bool nex>
struct _Mv_Ctr<Restriction::Trivial, nex> {
    _Mv_Ctr (_Mv_Ctr&&) noexcept(nex) = default;
};

template<bool nex>
struct _Mv_Ctr<Restriction::Constexpr, nex> {
    constexpr _Mv_Ctr (_Mv_Ctr&&) noexcept(nex) {};
};

///////////////////

template<Restriction r, bool nex>
struct _Cpy_Asg;

template<bool nex>
struct _Cpy_Asg<Restriction::ConstexprTrivial, nex> {
    constexpr _Cpy_Asg& operator= (_Cpy_Asg const&) noexcept(nex) = default;
};

template<bool nex>
struct _Cpy_Asg<Restriction::Deleted, nex> {
    _Cpy_Asg& operator= (_Cpy_Asg const&) noexcept(nex) = delete;
};

template<bool nex>
struct _Cpy_Asg<Restriction::Trivial, nex> {
    _Cpy_Asg& operator= (_Cpy_Asg const&) noexcept(nex) = default;
};

template<bool nex>
struct _Cpy_Asg<Restriction::Constexpr, nex> {
    constexpr _Cpy_Asg& operator= (_Cpy_Asg const&) noexcept(nex) { return *this; };
};

///////////////////

template<Restriction r, bool nex>
struct _Mv_Asg;

template<bool nex>
struct _Mv_Asg<Restriction::ConstexprTrivial, nex> {
    constexpr _Mv_Asg& operator= (_Mv_Asg&&) noexcept(nex) = default;
};

template<bool nex>
struct _Mv_Asg<Restriction::Deleted, nex> {
    _Mv_Asg& operator= (_Mv_Asg&&) noexcept(nex) = delete;
};

template<bool nex>
struct _Mv_Asg<Restriction::Trivial, nex> {
    _Mv_Asg& operator= (_Mv_Asg&&) noexcept(nex) = default;
};

template<bool nex>
struct _Mv_Asg<Restriction::Constexpr, nex> {
    constexpr _Mv_Asg& operator= (_Mv_Asg&&) noexcept(nex) { return *this; };
};

///////////////////

template<
    Restriction dctr, bool nex_dctr, 
    Restriction ictr, bool nex_ictr, 
    Restriction dtr, bool nex_dtr, 
    Restriction cpy_ctr, bool nex_cpy_ctr, 
    Restriction mv_ctr, bool nex_mv_ctr, 
    Restriction cpy_asg, bool nex_cpy_asg, 
    Restriction mv_asg, bool nex_mv_asg>
struct Observer : 
    _DCtr<dctr, nex_dctr>, 
    _ICtr<ictr, nex_ictr>, 
    _Dtr<dtr, nex_dtr>, 
    _Cpy_Ctr<cpy_ctr, nex_cpy_ctr>, 
    _Mv_Ctr<mv_ctr, nex_mv_ctr>, 
    _Cpy_Asg<cpy_asg, nex_cpy_asg>, 
    _Mv_Asg<mv_asg, nex_mv_asg> {};



template<typename T, std::size_t S>
using Queue = haz::Queue<T, S>;



using UniquePtrQEl = std::unique_ptr<int>;
using UniquePtrQ = Queue<UniquePtrQEl, 5>;

using IntQEl = int;
/*using IntQEl = Observer<
    Restriction::ConstexprTrivial, true,
    Restriction::Constexpr, true,
    Restriction::ConstexprTrivial, true,
    Restriction::ConstexprTrivial, true,
    Restriction::ConstexprTrivial, true,
    Restriction::ConstexprTrivial, true,
    Restriction::ConstexprTrivial, true
    >;*/
using IntQ = Queue<IntQEl, 5>;

using VectorQEl = std::vector<int>;
using VectorQ = Queue<VectorQEl, 5>;

using Array3QEl = std::array<int, 3>;
using Array3Q = Queue<Array3QEl, 5>;

using MutexQEl = std::mutex;
using MutexQ = Queue<MutexQEl, 5>;



template<typename T>
void error(T&& message) {
    ++error_count;
    std::lock_guard{ error_message_mutex };

    error_messages.emplace_back(std::forward<T>(message));
}


template<typename T>
using value_type_t = typename T::value_type;

void test_value_type() {
    if (! std::is_detected_exact_v<UniquePtrQEl, value_type_t, UniquePtrQ>) {
        error("test_value_type: error on UniquePtrQ");
    }

    if (! std::is_detected_exact_v<IntQEl, value_type_t, IntQ>) {
        error("test_value_type: error on IntQ");
    }

    if (! std::is_detected_exact_v<VectorQEl, value_type_t, VectorQ>) {
        error("test_value_type: error on VectorQ");
    }

    if (! std::is_detected_exact_v<Array3QEl, value_type_t, Array3Q>) {
        error("test_value_type: error on Array3Q");
    }

    if (! std::is_detected_exact_v<MutexQEl, value_type_t, MutexQ>) {
        error("test_value_type: error on MutexQ");
    }
} 



template<typename T>
using size_type_t = typename T::size_type;

void test_size_type() {
    if (! std::is_detected_exact_v<std::size_t, size_type_t, UniquePtrQ>) {
        error("test_size_type: error on UniquePtrQ");
    }

    if (! std::is_detected_exact_v<std::size_t, size_type_t, IntQ>) {
        error("test_size_type: error on IntQ");
    }

    if (! std::is_detected_exact_v<std::size_t, size_type_t, VectorQ>) {
        error("test_size_type: error on VectorQ");
    }

    if (! std::is_detected_exact_v<std::size_t, size_type_t, Array3Q>) {
        error("test_size_type: error on Array3Q");
    }

    if (! std::is_detected_exact_v<std::size_t, size_type_t, MutexQ>) {
        error("test_size_type: error on MutexQ");
    }
} 



template<typename T>
using difference_type_t = typename T::difference_type;

void test_difference_type() {
    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, UniquePtrQ>) {
        error("test_difference_type: error on UniquePtrQ");
    }

    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, IntQ>) {
        error("test_difference_type: error on IntQ");
    }

    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, VectorQ>) {
        error("test_difference_type: error on VectorQ");
    }

    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, Array3Q>) {
        error("test_difference_type: error on Array3Q");
    }

    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, MutexQ>) {
        error("test_difference_type: error on MutexQ");
    }
} 



template<typename T>
using reference_t = typename T::reference;

void test_reference() {
    if (! std::is_detected_exact_v<UniquePtrQEl&, reference_t, UniquePtrQ>) {
        error("test_reference: error on UniquePtrQ");
    }

    if (! std::is_detected_exact_v<IntQEl&, reference_t, IntQ>) {
        error("test_reference: error on IntQ");
    }

    if (! std::is_detected_exact_v<VectorQEl&, reference_t, VectorQ>) {
        error("test_reference: error on VectorQ");
    }

    if (! std::is_detected_exact_v<Array3QEl&, reference_t, Array3Q>) {
        error("test_reference: error on Array3Q");
    }

    if (! std::is_detected_exact_v<MutexQEl&, reference_t, MutexQ>) {
        error("test_reference: error on MutexQ");
    }
} 



template<typename T>
using const_reference_t = typename T::const_reference;

void test_const_reference() {
    if (! std::is_detected_exact_v<UniquePtrQEl const&, const_reference_t, UniquePtrQ>) {
        error("test_const_reference: error on UniquePtrQ");
    }

    if (! std::is_detected_exact_v<IntQEl const&, const_reference_t, IntQ>) {
        error("test_const_reference: error on IntQ");
    }

    if (! std::is_detected_exact_v<VectorQEl const&, const_reference_t, VectorQ>) {
        error("test_const_reference: error on VectorQ");
    }

    if (! std::is_detected_exact_v<Array3QEl const&, const_reference_t, Array3Q>) {
        error("test_const_reference: error on Array3Q");
    }

    if (! std::is_detected_exact_v<MutexQEl const&, const_reference_t, MutexQ>) {
        error("test_const_reference: error on MutexQ");
    }
} 



template<typename T>
using pointer_t = typename T::pointer;

void test_pointer() {
    if (! std::is_detected_exact_v<UniquePtrQEl*, pointer_t, UniquePtrQ>) {
        error("test_pointer: error on UniquePtrQ");
    }

    if (! std::is_detected_exact_v<IntQEl*, pointer_t, IntQ>) {
        error("test_pointer: error on IntQ");
    }

    if (! std::is_detected_exact_v<VectorQEl*, pointer_t, VectorQ>) {
        error("test_pointer: error on VectorQ");
    }

    if (! std::is_detected_exact_v<Array3QEl*, pointer_t, Array3Q>) {
        error("test_pointer: error on Array3Q");
    }

    if (! std::is_detected_exact_v<MutexQEl*, pointer_t, MutexQ>) {
        error("test_pointer: error on MutexQ");
    }
} 



template<typename T>
using const_pointer_t = typename T::const_pointer;

void test_const_pointer() {
    if (! std::is_detected_exact_v<UniquePtrQEl const*, const_pointer_t, UniquePtrQ>) {
        error("test_const_pointer: error on UniquePtrQ");
    }

    if (! std::is_detected_exact_v<IntQEl const*, const_pointer_t, IntQ>) {
        error("test_const_pointer: error on IntQ");
    }

    if (! std::is_detected_exact_v<VectorQEl const*, const_pointer_t, VectorQ>) {
        error("test_const_pointer: error on VectorQ");
    }

    if (! std::is_detected_exact_v<Array3QEl const*, const_pointer_t, Array3Q>) {
        error("test_const_pointer: error on Array3Q");
    }

    if (! std::is_detected_exact_v<MutexQEl const*, const_pointer_t, MutexQ>) {
        error("test_const_pointer: error on MutexQ");
    }
} 



template<typename T>
using iterator_t = typename T::iterator;

void test_iterator() {
    if (! std::is_detected_v<iterator_t, UniquePtrQ>) {
        error("test_iterator: error on UniquePtrQ");
    }

    if (! std::is_detected_v<iterator_t, IntQ>) {
        error("test_iterator: error on IntQ");
    }

    if (! std::is_detected_v<iterator_t, VectorQ>) {
        error("test_iterator: error on VectorQ");
    }

    if (! std::is_detected_v<iterator_t, Array3Q>) {
        error("test_iterator: error on Array3Q");
    }

    if (! std::is_detected_v<iterator_t, MutexQ>) {
        error("test_iterator: error on MutexQ");
    }
} 



template<typename T>
using const_iterator_t = typename T::const_iterator;

void test_const_iterator() {
    if (! std::is_detected_v<const_iterator_t, UniquePtrQ>) {
        error("test_const_iterator: error on UniquePtrQ");
    }

    if (! std::is_detected_v<const_iterator_t, IntQ>) {
        error("test_const_iterator: error on IntQ");
    }

    if (! std::is_detected_v<const_iterator_t, VectorQ>) {
        error("test_const_iterator: error on VectorQ");
    }

    if (! std::is_detected_v<const_iterator_t, Array3Q>) {
        error("test_const_iterator: error on Array3Q");
    }

    if (! std::is_detected_v<const_iterator_t, MutexQ>) {
        error("test_const_iterator: error on MutexQ");
    }
} 



template<typename T>
using reverse_iterator_t = typename T::reverse_iterator;

void test_reverse_iterator() {
    if (! std::is_detected_v<reverse_iterator_t, UniquePtrQ>) {
        error("test_reverse_iterator: error on UniquePtrQ");
    }

    if (! std::is_detected_v<reverse_iterator_t, IntQ>) {
        error("test_reverse_iterator: error on IntQ");
    }

    if (! std::is_detected_v<reverse_iterator_t, VectorQ>) {
        error("test_reverse_iterator: error on VectorQ");
    }

    if (! std::is_detected_v<reverse_iterator_t, Array3Q>) {
        error("test_reverse_iterator: error on Array3Q");
    }

    if (! std::is_detected_v<reverse_iterator_t, MutexQ>) {
        error("test_reverse_iterator: error on MutexQ");
    }
} 



template<typename T>
using const_reverse_iterator_t = typename T::const_reverse_iterator;

void test_const_reverse_iterator() {
    if (! std::is_detected_v<const_reverse_iterator_t, UniquePtrQ>) {
        error("test_const_reverse_iterator: error on UniquePtrQ");
    }

    if (! std::is_detected_v<const_reverse_iterator_t, IntQ>) {
        error("test_const_reverse_iterator: error on IntQ");
    }

    if (! std::is_detected_v<const_reverse_iterator_t, VectorQ>) {
        error("test_const_reverse_iterator: error on VectorQ");
    }

    if (! std::is_detected_v<const_reverse_iterator_t, Array3Q>) {
        error("test_const_reverse_iterator: error on Array3Q");
    }

    if (! std::is_detected_v<const_reverse_iterator_t, MutexQ>) {
        error("test_const_reverse_iterator: error on MutexQ");
    }
}



template<bool CS, bool RT, typename C, typename M, typename N>
void _test_constructor_default(M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c;
        constexpr typename C::size_type size = c.size(); 
        if constexpr (size != 0) {
            error(std::forward<M>(cs_message));
        }
    }

    if constexpr (RT) {
        C c;
        typename C::size_type size = c.size(); 
        if (size != 0) {
            error(std::forward<N>(rt_message));
        }
    }
}

void test_constructor_default() {
    _test_constructor_default<false, true, UniquePtrQ>(
        "test_constructor_default: UniquePtrQ compile-time's size is not 0",
        "test_constructor_default: UniquePtrQ runtime's size is not 0");
    _test_constructor_default<true, true, IntQ>(
        "test_constructor_default: IntQ compile-time's size is not 0",
        "test_constructor_default: IntQ runtime's size is not 0");
    _test_constructor_default<false, true, VectorQ>(
        "test_constructor_default: VectorQ compile-time's size is not 0",
        "test_constructor_default: VectorQ runtime's size is not 0");
    _test_constructor_default<true, true, Array3Q>(
        "test_constructor_default: Array3Q compile-time's size is not 0",
        "test_constructor_default: Array3Q runtime's size is not 0");
    _test_constructor_default<true, true, MutexQ>(
        "test_constructor_default: MutexQ compile-time's size is not 0",
        "test_constructor_default: MutexQ runtime's size is not 0");
}



template<bool CS, bool RT, typename C, typename M, typename N>
void _test_constructor_init_default_n(M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c(typename C::size_type{5});
        constexpr typename C::size_type size = c.size();

        if constexpr (size != 5) {
            error(std::forward<M>(cs_message));
        }
    }

    if constexpr (RT) {
        C c(typename C::size_type{5});
        typename C::size_type size = c.size();

        if (size != 5) {
            error(std::forward<N>(rt_message));
        }
    }
}



void test_constructor_init_default_n() {
    _test_constructor_init_default_n<false, true, UniquePtrQ>(
        "test_constructor_init_default_n: UniquePtrQ compile-time's size is not 5",
        "test_constructor_init_default_n: UniquePtrQ runtime's size is not 5");
    _test_constructor_init_default_n<true, true, IntQ>(
        "test_constructor_init_default_n: IntQ compile-time's size is not 5",
        "test_constructor_init_default_n: IntQ runtime's size is not 5");
    _test_constructor_init_default_n<false, true, VectorQ>(
        "test_constructor_init_default_n: VectorQ compile-time's size is not 5",
        "test_constructor_init_default_n: VectorQ runtime's size is not 5");
    _test_constructor_init_default_n<true, true, Array3Q>(
        "test_constructor_init_default_n: Array3Q compile-time's size is not 5",
        "test_constructor_init_default_n: Array3Q runtime's size is not 5");
    _test_constructor_init_default_n<true, true, MutexQ>(
        "test_constructor_init_default_n: MutexQ compile-time's size is not 5",
        "test_constructor_init_default_n: MutexQ runtime's size is not 5");
}



template<bool CS, bool RT, typename C, typename E, typename F, typename M, typename N>
void _test_constructor_init_copy_n(F&& make, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr E e = make();
        constexpr C c(typename C::size_type{5}, e);
        constexpr typename C::size_type size = c.size();

        if constexpr (size != 5 || e != c[0] || e != c[4]) {
            error(std::forward<M>(cs_message));
        }
    }

    if constexpr (RT) {
        E e = make();
        C c(typename C::size_type{5}, e);
        typename C::size_type size = c.size();

        if (size != 5 || e != c[0] || e != c[4]) {
            error(std::forward<N>(rt_message));
        }
    }
}



constexpr bool operator !=(Array3QEl const& lhs, Array3QEl const& rhs) {
    for(std::size_t i{0}; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return true;
        }
    }
    return false;
}



void test_constructor_init_copy_n() {
    _test_constructor_init_copy_n<true, true, IntQ, IntQEl>([] () -> IntQEl { return 0xBADC0DE; },
        "test_constructor_init_copy_n: IntQ compile-time's size is not 5, or the copy did something wrong",
        "test_constructor_init_copy_n: IntQ runtime's size is not 5, or the copy did something wrong");
    _test_constructor_init_copy_n<false, true, VectorQ, VectorQEl>([] () -> VectorQEl { return { 0, 1, 2 }; },
        "test_constructor_init_copy_n: VectorQ compile-time's size is not 5, or the copy did something wrong",
        "test_constructor_init_copy_n: VectorQ runtime's size is not 5, or the copy did something wrong");
    _test_constructor_init_copy_n<true, true, Array3Q, Array3QEl>([] () -> Array3QEl { return { 0, 1, 2 }; },
        "test_constructor_init_copy_n: Array3Q compile-time's size is not 5, or the copy did something wrong",
        "test_constructor_init_copy_n: Array3Q runtime's size is not 5, or the copy did something wrong");
}



template<bool CS, bool RT, typename C, typename E, typename F, typename M, typename N>
void _test_constructor_range(F&& make, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr std::array<E, 4> es{ make(0), make(1), make(2), make(3) };
        constexpr C c(std::begin(es), std::end(es));
        constexpr typename C::size_type size = c.size();

        if constexpr (size != 4 || *std::begin(es) != c[0] || *(std::begin(es) + 3) != c[3]) {
            error(std::forward<M>(cs_message));
        }
    }

    if constexpr (RT) {
        std::array<E, 4> es{ make(0), make(1), make(2), make(3) };
        C c(std::begin(es), std::end(es));
        typename C::size_type size = c.size();

        if (size != 4 || *std::begin(es) != c[0] || *(std::begin(es) + 3) != c[3]) {
            error(std::forward<N>(rt_message));
        }
    }
}



void test_constructor_range() {
    _test_constructor_range<true, true, IntQ, IntQEl>([] (int idx) -> IntQEl { return 0xBADC0DE << idx; },
        "test_constructor_range: IntQ compile-time's size is not 4, or the copy of the range did something wrong",
        "test_constructor_range: IntQ runtime's size is not 4, or the copy of the range did something wrong");
    _test_constructor_range<false, true, VectorQ, VectorQEl>([] (int idx) -> VectorQEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_constructor_range: VectorQ compile-time's size is not 4, or the copy of the range did something wrong",
        "test_constructor_range: VectorQ runtime's size is not 4, or the copy of the range did something wrong");
    _test_constructor_range<true, true, Array3Q, Array3QEl>([] (int idx) -> Array3QEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_constructor_range: Array3Q compile-time's size is not 4, or the copy of the range did something wrong",
        "test_constructor_range: Array3Q runtime's size is not 4, or the copy of the range did something wrong");
}



template<bool CS, bool RT, typename C, typename E, typename F, typename M, typename N>
void _test_constructor_initializer_list(F&& make, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c{ make(0), make(1), make(2), make(3) };
        constexpr typename C::size_type size = c.size();

        if constexpr (size != 4 || make(0) != c[0] || make(3) != c[3]) {
            error(std::forward<M>(cs_message));
        }
    }

    if constexpr (RT) {
        C c{ make(0), make(1), make(2), make(3) };
        typename C::size_type size = c.size();

        if (size != 4 || make(0) != c[0] || make(3) != c[3]) {
            error(std::forward<N>(rt_message));
        }
    }
}



void test_constructor_initializer_list() {
    _test_constructor_initializer_list<true, true, IntQ, IntQEl>([] (int idx) -> IntQEl { return 0xBADC0DE << idx; },
        "test_constructor_initializer_list: IntQ compile-time's size is not 4, or the copy of the range did something wrong",
        "test_constructor_initializer_list: IntQ runtime's size is not 4, or the copy of the range did something wrong");
    _test_constructor_initializer_list<false, true, VectorQ, VectorQEl>([] (int idx) -> VectorQEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_constructor_initializer_list: VectorQ compile-time's size is not 4, or the copy of the range did something wrong",
        "test_constructor_initializer_list: VectorQ runtime's size is not 4, or the copy of the range did something wrong");
    _test_constructor_initializer_list<true, true, Array3Q, Array3QEl>([] (int idx) -> Array3QEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_constructor_initializer_list: Array3Q compile-time's size is not 4, or the copy of the range did something wrong",
        "test_constructor_initializer_list: Array3Q runtime's size is not 4, or the copy of the range did something wrong");
}



template<typename C, typename E, typename F>
constexpr C _test_push_back_copy_helper(F&& make) {
    C c;
    E e0 = make(0);
    E e1 = make(1);
    E e2 = make(2);
    c.push_back(e0);
    c.push_back(e1);
    c.push_back(e2);
    return c;
}



template<bool CS, bool RT, typename C, typename E, typename F, typename M, typename N>
void _test_push_back_copy(F&& make, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c = _test_push_back_copy_helper<C, E>(make);
        constexpr typename C::size_type size = c.size();

        if constexpr (size != 3 || make(0) != c[0] || make(1) != c[1] || make(2) != c[2]) {
            error(std::forward<M>(cs_message));
        }
    }

    if constexpr (RT) {
        C c = _test_push_back_copy_helper<C, E>(make);
        typename C::size_type size = c.size();

        if (size != 3 || make(0) != c[0] || make(1) != c[1] || make(2) != c[2]) {
            error(std::forward<N>(rt_message));
        }
    }
}



void test_push_back_copy() {
    _test_push_back_copy<true, true, IntQ, IntQEl>([] (int idx) -> IntQEl { return 0xBADC0DE << idx; },
        "test_push_back_copy: IntQ compile-time's size is not 3, or the elements are wrongly copied",
        "test_push_back_copy: IntQ runtime's size is not 3, or the elements are wrongly copied");
    _test_push_back_copy<false, true, VectorQ, VectorQEl>([] (int idx) -> VectorQEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_push_back_copy: VectorQ compile-time's size is not 3, or the elements are wrongly copied",
        "test_push_back_copy: VectorQ runtime's size is not 3, or the elements are wrongly copied");
    _test_push_back_copy<true, true, Array3Q, Array3QEl>([] (int idx) -> Array3QEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_push_back_copy: Array3Q compile-time's size is not 3, or the elements are wrongly copied",
        "test_push_back_copy: Array3Q runtime's size is not 3, or the elements are wrongly copied");
}



template<typename C, typename F>
constexpr C _test_push_back_move_helper(F&& make) {
    C c;
    c.push_back(std::move(make(0)));
    c.push_back(std::move(make(1)));
    c.push_back(std::move(make(2)));
    return c;
}



template<bool CS, bool RT, typename C, typename E, bool D, typename F, typename M, typename N>
void _test_push_back_move(F&& make, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c = _test_push_back_move_helper<C>(make);
        constexpr typename C::size_type size = c.size();

        if constexpr (D) {
            if constexpr (size != 3 || *make(0) != *c[0] || *make(1) != *c[1] || *make(2) != *c[2]) {
                error(std::forward<M>(cs_message));
            }
        } else {
            if constexpr (size != 3 || make(0) != c[0] || make(1) != c[1] || make(2) != c[2]) {
                error(std::forward<M>(cs_message));
            }
        }
    }

    if constexpr (RT) {
        C c = _test_push_back_move_helper<C>(make);
        typename C::size_type size = c.size();

        if constexpr (D) {
            if (size != 3 || *make(0) != *c[0] || *make(1) != *c[1] || *make(2) != *c[2]) {
                error(std::forward<N>(rt_message));
            }
        } else {
            if (size != 3 || make(0) != c[0] || make(1) != c[1] || make(2) != c[2]) {
                error(std::forward<N>(rt_message));
            }
        }
    }
}



void test_push_back_move() {
    _test_push_back_move<false, true, UniquePtrQ, UniquePtrQEl, true>([] (int idx) -> UniquePtrQEl { return std::make_unique<int>(0xBADC0DE << idx); },
        "test_push_back_move: UniquePtrQ compile-time's size is not 3, or the elements are wrongly moved",
        "test_push_back_move: UniquePtrQ runtime's size is not 3, or the elements are wrongly moved");
    _test_push_back_move<true, true, IntQ, IntQEl, false>([] (int idx) -> IntQEl { return 0xBADC0DE << idx; },
        "test_push_back_move: IntQ compile-time's size is not 3, or the elements are wrongly moved",
        "test_push_back_move: IntQ runtime's size is not 3, or the elements are wrongly moved");
    _test_push_back_move<false, true, VectorQ, VectorQEl, false>([] (int idx) -> VectorQEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_push_back_move: VectorQ compile-time's size is not 3, or the elements are wrongly moved",
        "test_push_back_move: VectorQ runtime's size is not 3, or the elements are wrongly moved");
    _test_push_back_move<true, true, Array3Q, Array3QEl, false>([] (int idx) -> Array3QEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_push_back_move: Array3Q compile-time's size is not 3, or the elements are wrongly moved",
        "test_push_back_move: Array3Q runtime's size is not 3, or the elements are wrongly moved");
}



template<bool CS, bool RT, typename C, typename E, bool D, typename F, typename M, typename N>
void _test_copy_constructor(F&& make, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C o{ make(0), make(1), make(2), make(3) };
        constexpr C c(o);

        if constexpr (D) {
            if constexpr (c.size() != o.size() || *o[0] != *c[0] || *o[1] != *c[1] || *o[2] != *c[2] || *o[3] != *c[3]) {
                error(std::forward<M>(cs_message));
            }
        } else {
            if constexpr (c.size() != o.size() || o[0] != c[0] || o[1] != c[1] || o[2] != c[2] || o[3] != c[3]) {
                error(std::forward<M>(cs_message));
            }
        }
    }

    if constexpr (RT) {
        C o{ make(0), make(1), make(2), make(3) };
        C c(o);

        if constexpr (D) {
            if (c.size() != o.size() || *o[0] != *c[0] || *o[1] != *c[1] || *o[2] != *c[2] || *o[3] != *c[3]) {
                error(std::forward<N>(rt_message));
            }
        } else {
            if (c.size() != o.size() || o[0] != c[0] || o[1] != c[1] || o[2] != c[2] || o[3] != c[3]) {
                error(std::forward<N>(rt_message));
            }
        }
    }
}



void test_copy_constructor() {
    _test_copy_constructor<true, true, IntQ, IntQEl, false>([] (int idx) -> IntQEl { return 0xBADC0DE << idx; },
        "test_copy_constructor: IntQ compile-time's size is not 4, or the elements are wrongly copied",
        "test_copy_constructor: IntQ runtime's size is not 4, or the elements are wrongly copied");
    _test_copy_constructor<false, true, VectorQ, VectorQEl, false>([] (int idx) -> VectorQEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_copy_constructor: VectorQ compile-time's size is not 4, or the elements are wrongly copied",
        "test_copy_constructor: VectorQ runtime's size is not 4, or the elements are wrongly copied");
    _test_copy_constructor<true, true, Array3Q, Array3QEl, false>([] (int idx) -> Array3QEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_copy_constructor: Array3Q compile-time's size is not 4, or the elements are wrongly copied",
        "test_copy_constructor: Array3Q runtime's size is not 4, or the elements are wrongly copied");
}



template<bool CS, bool RT, typename C, typename E, bool D, typename F, typename M, typename N>
void _test_move_constructor(F&& make, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c(std::move(_test_push_back_move_helper<C>(make)));

        if constexpr (D) {
            if constexpr (c.size() != 4 || *make(0) != *c[0] || *make(1) != *c[1] || *make(2) != *c[2] || *make(3) != *c[3]) {
                error(std::forward<M>(cs_message));
            }
        } else {
            if constexpr (c.size() != 4 || make(0) != c[0] || make(1) != c[1] || make(2) != c[2] || make(3) != c[3]) {
                error(std::forward<M>(cs_message));
            }
        }
    }

    if constexpr (RT) {
        C c(std::move(_test_push_back_move_helper<C>(make)));

        if constexpr (D) {
            if (c.size() != 4 || *make(0) != *c[0] || *make(1) != *c[1] || *make(2) != *c[2] || *make(3) != *c[3]) {
                error(std::forward<N>(rt_message));
            }
        } else {
            if (c.size() != 4 || make(0) != c[0] || make(1) != c[1] || make(2) != c[2] || make(3) != c[3]) {
                error(std::forward<N>(rt_message));
            }
        }
    }
}



void test_move_constructor() {
    _test_move_constructor<false, true, UniquePtrQ, UniquePtrQEl, true>([] (int idx) -> UniquePtrQEl { return std::make_unique<int>(0xBADC0DE << idx); },
        "test_move_constructor: IntQ compile-time's size is not 4, or the elements are wrongly moved",
        "test_move_constructor: IntQ runtime's size is not 4, or the elements are wrongly moved");
    _test_move_constructor<true, true, IntQ, IntQEl, false>([] (int idx) -> IntQEl { return 0xBADC0DE << idx; },
        "test_move_constructor: IntQ compile-time's size is not 4, or the elements are wrongly moved",
        "test_move_constructor: IntQ runtime's size is not 4, or the elements are wrongly moved");
    _test_move_constructor<false, true, VectorQ, VectorQEl, false>([] (int idx) -> VectorQEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_move_constructor: VectorQ compile-time's size is not 4, or the elements are wrongly moved",
        "test_move_constructor: VectorQ runtime's size is not 4, or the elements are wrongly moved");
    _test_move_constructor<true, true, Array3Q, Array3QEl, false>([] (int idx) -> Array3QEl { return { idx, idx * idx, idx * idx * idx }; },
        "test_move_constructor: Array3Q compile-time's size is not 4, or the elements are wrongly moved",
        "test_move_constructor: Array3Q runtime's size is not 4, or the elements are wrongly moved");
}




template<bool CS, bool RT, typename C, typename M, typename N>
void _test_capacity(M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c;
        if constexpr (c.capacity() != 5) {
            error(std::forward<M>(cs_message));
        }
    } 
    
    if constexpr (RT) {
        C c;
        if (c.capacity() != 5) {
            error(std::forward<N>(rt_message));
        }
    }
}



void test_capacity() {
    _test_capacity<false, true, UniquePtrQ>(
        "test_capacity: UniquePtrQ compile-time's capacity is not 5",
        "test_capacity: UniquePtrQ runtime's capacity is not 5");
    _test_capacity<true, true, IntQ>(
        "test_capacity: IntQ compile-time's capacity is not 5",
        "test_capacity: IntQ runtime's capacity is not 5");
    _test_capacity<false, true, VectorQ>(
        "test_capacity: VectorQ compile-time's capacity is not 5",
        "test_capacity: VectorQ runtime's capacity is not 5");
    _test_capacity<true, true, Array3Q>(
        "test_capacity: Array3Q compile-time's capacity is not 5",
        "test_capacity: Array3Q runtime's capacity is not 5");
    _test_capacity<true, true, MutexQ>(
        "test_capacity: MutexQ compile-time's capacity is not 5",
        "test_capacity: MutexQ runtime's capacity is not 5");
}



template<bool CS, bool RT, typename C, typename M, typename N>
void _test_max_size(M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c;
        if constexpr (c.max_size() != 5) {
            error(std::forward<M>(cs_message));
        }
    } 
    
    if constexpr (RT) {
        C c;
        if (c.max_size() != 5) {
            error(std::forward<N>(rt_message));
        }
    }
}



void test_max_size() {
    _test_max_size<false, true, UniquePtrQ>(
        "test_max_size: UniquePtrQ compile-time's max size is not 5",
        "test_max_size: UniquePtrQ runtime's max size is not 5");
    _test_max_size<true, true, IntQ>(
        "test_max_size: IntQ compile-time's max size is not 5",
        "test_max_size: IntQ runtime's max size is not 5");
    _test_max_size<false, true, VectorQ>(
        "test_max_size: VectorQ compile-time's max size is not 5",
        "test_max_size: VectorQ runtime's max size is not 5");
    _test_max_size<true, true, Array3Q>(
        "test_max_size: Array3Q compile-time's max size is not 5",
        "test_max_size: Array3Q runtime's max size is not 5");
    _test_max_size<true, true, MutexQ>(
        "test_max_size: MutexQ compile-time's max size is not 5",
        "test_max_size: MutexQ runtime's max size is not 5");
}



template<bool CS, bool RT, typename C, typename M, typename N>
void _test_size(M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c;
        if constexpr (c.size() != 0) {
            error(std::forward<M>(cs_message));
        }
    } 
    
    if constexpr (RT) {
        C c;
        if (c.size() != 0) {
            error(std::forward<N>(rt_message));
        }
    }
}



template<bool CS, bool RT, typename C, typename F, typename M, typename N>
void _test_size_after_emplace_back(F&& f, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c = [&] () { C _c; _c.emplace_back(f()); return _c; }();
        if constexpr (c.size() != 1) {
            error(std::forward<M>(cs_message));
        }
    } 
    
    if constexpr (RT) {
        C c;
        c.emplace_back(f());
        if (c.size() != 1) {
            error(std::forward<N>(rt_message));
        }
    }
}



template<bool CS, bool RT, typename C, typename F, typename M, typename N>
void _test_size_after_emplace_back_and_pop_front(F&& f, M&& cs_message, N&& rt_message) {
    if constexpr (CS) {
        constexpr C c = [&] () { C _c; _c.emplace_back(f()); _c.pop_front(); return _c; }();
        if constexpr (c.size() != 0) {
            error(std::forward<M>(cs_message));
        }
    } 
    
    if constexpr (RT) {
        C c;
        c.emplace_back(f());
        c.pop_front();
        if (c.size() != 0) {
            error(std::forward<N>(rt_message));
        }
    }
}



void test_size() {
    _test_size<false, true, UniquePtrQ>(
        "test_size: UniquePtrQ compile-time's size is not 0",
        "test_size: UniquePtrQ runtime's size is not 0");
    _test_size<true, true, IntQ>(
        "test_size: IntQ compile-time's size is not 0",
        "test_size: IntQ runtime's size is not 0");
    _test_size<false, true, VectorQ>(
        "test_size: VectorQ compile-time's size is not 0",
        "test_size: VectorQ runtime's size is not 0");
    _test_size<true, true, Array3Q>(
        "test_size: Array3Q compile-time's size is not 0",
        "test_size: Array3Q runtime's size is not 0");
    _test_size<false, true, MutexQ>(
        "test_size: MutexQ compile-time's size is not 0",
        "test_size: MutexQ runtime's size is not 0");

    _test_size_after_emplace_back<false, true, UniquePtrQ>( [] () { return std::make_unique<int>(0xBADC0DE); },
        "test_size_after_emplace_back: UniquePtrQ compile-time's size is not 1",
        "test_size_after_emplace_back: UniquePtrQ runtime's size is not 1");
    _test_size_after_emplace_back<true, true, IntQ>( [] () { return 0xBADC0DE; },
        "test_size_after_emplace_back: IntQ compile-time's size is not ",
        "test_size_after_emplace_back: IntQ runtime's size is not ");
    _test_size_after_emplace_back<false, true, VectorQ>( [] () { return VectorQEl{ 0xBADC0DE }; },
        "test_size_after_emplace_back: VectorQ compile-time's size is not ",
        "test_size_after_emplace_back: VectorQ runtime's size is not ");
    _test_size_after_emplace_back<true, true, Array3Q>( [] () { return Array3QEl{ 0xBADC0DE };; },
        "test_size_after_emplace_back: Array3Q compile-time's size is not ",
        "test_size_after_emplace_back: Array3Q runtime's size is not ");

    _test_size_after_emplace_back_and_pop_front<false, true, UniquePtrQ>( [] () { return std::make_unique<int>(0xBADC0DE); },
        "test_size_after_emplace_back_and_pop_front: UniquePtrQ compile-time's size is not 0",
        "test_size_after_emplace_back_and_pop_front: UniquePtrQ runtime's size is not 0");
    _test_size_after_emplace_back_and_pop_front<true, true, IntQ>( [] () { return 0xBADC0DE; },
        "test_size_after_emplace_back_and_pop_front: IntQ compile-time's size is not 0",
        "test_size_after_emplace_back_and_pop_front: IntQ runtime's size is not 0");
    _test_size_after_emplace_back_and_pop_front<false, true, VectorQ>( [] () { return VectorQEl{ 0xBADC0DE }; },
        "test_size_after_emplace_back_and_pop_front: VectorQ compile-time's size is not 0",
        "test_size_after_emplace_back_and_pop_front: VectorQ runtime's size is not 0");
    _test_size_after_emplace_back_and_pop_front<true, true, Array3Q>( [] () { return Array3QEl{ 0xBADC0DE };; },
        "test_size_after_emplace_back_and_pop_front: Array3Q compile-time's size is not 0",
        "test_size_after_emplace_back_and_pop_front: Array3Q runtime's size is not 0");
}



void test_empty() {

}



void test_full() {

}






int main() {
    using func_t = void(*)();

    std::cout << std::is_trivially_constructible_v<int, int> << '\n';

    struct X {
        int i;
    };

    struct Y {
        Y(int ii) noexcept : i(ii) {};
        int i;
    };

    struct Z {
        Z(int) noexcept {};
        int i;
    };

    std::cout << std::is_trivially_constructible_v<X, int> << '\n';
    std::cout << std::is_trivially_constructible_v<Y, int> << '\n';
    std::cout << std::is_trivially_constructible_v<Z, int> << '\n';

    std::vector<func_t> funcs {
        // Traits
        test_value_type,
        test_size_type,
        test_difference_type,
        test_reference,
        test_const_reference,
        test_pointer,
        test_const_pointer,
        test_iterator,
        test_const_iterator,
        test_reverse_iterator,
        test_const_reverse_iterator,

        // Constructors
        test_constructor_default,
        test_constructor_init_default_n,
        test_constructor_init_copy_n,
        test_constructor_range,
        test_constructor_initializer_list,

        // Special members

        // Capacity
        test_capacity,
        test_max_size,
        test_size,
        test_empty,
        test_full,

        // Element Access

        // Modifier
        test_push_back_copy,
        test_push_back_move,

        // Iterators

        // Non-member functions
    };

    std::vector<std::thread> threads;
    threads.reserve(funcs.size());

    for(auto func : funcs) {
        threads.emplace_back(func);
    }

    for(auto& t : threads) {
        t.join();
    }

    if (error_messages.empty()) {
        std::cout << "\033[32;1mAll good :)\033[0m\n";
    } else {
        for(auto const& m : error_messages) {
            std::cerr << m << '\n';
        }
    }

    return error_count;
}