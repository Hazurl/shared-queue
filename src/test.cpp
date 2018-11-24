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



template<typename T, std::size_t S>
using Queue = haz::Queue<T, S>;



using MoveOnlyQueueElement = std::unique_ptr<int>;
using MoveOnlyQueue = Queue<MoveOnlyQueueElement, 5>;

using DefaultQueueElement = int;
using DefaultQueue = Queue<DefaultQueueElement, 5>;

using CopyFastMoveQueueElement = std::vector<int>;
using CopyFastMoveQueue = Queue<CopyFastMoveQueueElement, 5>;

using DeepCopyQueueElement = std::array<int, 3>;
using DeepCopyQueue = Queue<DeepCopyQueueElement, 5>;



template<typename T>
void error(T&& message) {
    ++error_count;
    std::lock_guard{ error_message_mutex };

    error_messages.emplace_back(std::forward<T>(message));
}


template<typename T>
using value_type_t = typename T::value_type;

void test_value_type() {
    if (! std::is_detected_exact_v<MoveOnlyQueueElement, value_type_t, MoveOnlyQueue>) {
        error("test_value_type: error on MoveOnlyQueue");
    }

    if (! std::is_detected_exact_v<DefaultQueueElement, value_type_t, DefaultQueue>) {
        error("test_value_type: error on DefaultQueue");
    }

    if (! std::is_detected_exact_v<CopyFastMoveQueueElement, value_type_t, CopyFastMoveQueue>) {
        error("test_value_type: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_exact_v<DeepCopyQueueElement, value_type_t, DeepCopyQueue>) {
        error("test_value_type: error on DeepCopyQueue");
    }
} 



template<typename T>
using size_type_t = typename T::size_type;

void test_size_type() {
    if (! std::is_detected_exact_v<std::size_t, size_type_t, MoveOnlyQueue>) {
        error("test_size_type: error on MoveOnlyQueue");
    }

    if (! std::is_detected_exact_v<std::size_t, size_type_t, DefaultQueue>) {
        error("test_size_type: error on DefaultQueue");
    }

    if (! std::is_detected_exact_v<std::size_t, size_type_t, CopyFastMoveQueue>) {
        error("test_size_type: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_exact_v<std::size_t, size_type_t, DeepCopyQueue>) {
        error("test_size_type: error on DeepCopyQueue");
    }
} 



template<typename T>
using difference_type_t = typename T::difference_type;

void test_difference_type() {
    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, MoveOnlyQueue>) {
        error("test_difference_type: error on MoveOnlyQueue");
    }

    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, DefaultQueue>) {
        error("test_difference_type: error on DefaultQueue");
    }

    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, CopyFastMoveQueue>) {
        error("test_difference_type: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_exact_v<std::ptrdiff_t, difference_type_t, DeepCopyQueue>) {
        error("test_difference_type: error on DeepCopyQueue");
    }
} 



template<typename T>
using reference_t = typename T::reference;

void test_reference() {
    if (! std::is_detected_exact_v<MoveOnlyQueueElement&, reference_t, MoveOnlyQueue>) {
        error("test_reference: error on MoveOnlyQueue");
    }

    if (! std::is_detected_exact_v<DefaultQueueElement&, reference_t, DefaultQueue>) {
        error("test_reference: error on DefaultQueue");
    }

    if (! std::is_detected_exact_v<CopyFastMoveQueueElement&, reference_t, CopyFastMoveQueue>) {
        error("test_reference: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_exact_v<DeepCopyQueueElement&, reference_t, DeepCopyQueue>) {
        error("test_reference: error on DeepCopyQueue");
    }
} 



template<typename T>
using const_reference_t = typename T::const_reference;

void test_const_reference() {
    if (! std::is_detected_exact_v<MoveOnlyQueueElement const&, const_reference_t, MoveOnlyQueue>) {
        error("test_const_reference: error on MoveOnlyQueue");
    }

    if (! std::is_detected_exact_v<DefaultQueueElement const&, const_reference_t, DefaultQueue>) {
        error("test_const_reference: error on DefaultQueue");
    }

    if (! std::is_detected_exact_v<CopyFastMoveQueueElement const&, const_reference_t, CopyFastMoveQueue>) {
        error("test_const_reference: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_exact_v<DeepCopyQueueElement const&, const_reference_t, DeepCopyQueue>) {
        error("test_const_reference: error on DeepCopyQueue");
    }
} 



template<typename T>
using pointer_t = typename T::pointer;

void test_pointer() {
    if (! std::is_detected_exact_v<MoveOnlyQueueElement*, pointer_t, MoveOnlyQueue>) {
        error("test_pointer: error on MoveOnlyQueue");
    }

    if (! std::is_detected_exact_v<DefaultQueueElement*, pointer_t, DefaultQueue>) {
        error("test_pointer: error on DefaultQueue");
    }

    if (! std::is_detected_exact_v<CopyFastMoveQueueElement*, pointer_t, CopyFastMoveQueue>) {
        error("test_pointer: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_exact_v<DeepCopyQueueElement*, pointer_t, DeepCopyQueue>) {
        error("test_pointer: error on DeepCopyQueue");
    }
} 



template<typename T>
using const_pointer_t = typename T::const_pointer;

void test_const_pointer() {
    if (! std::is_detected_exact_v<MoveOnlyQueueElement const*, const_pointer_t, MoveOnlyQueue>) {
        error("test_const_pointer: error on MoveOnlyQueue");
    }

    if (! std::is_detected_exact_v<DefaultQueueElement const*, const_pointer_t, DefaultQueue>) {
        error("test_const_pointer: error on DefaultQueue");
    }

    if (! std::is_detected_exact_v<CopyFastMoveQueueElement const*, const_pointer_t, CopyFastMoveQueue>) {
        error("test_const_pointer: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_exact_v<DeepCopyQueueElement const*, const_pointer_t, DeepCopyQueue>) {
        error("test_const_pointer: error on DeepCopyQueue");
    }
} 



template<typename T>
using iterator_t = typename T::iterator;

void test_iterator() {
    if (! std::is_detected_v<iterator_t, MoveOnlyQueue>) {
        error("test_iterator: error on MoveOnlyQueue");
    }

    if (! std::is_detected_v<iterator_t, DefaultQueue>) {
        error("test_iterator: error on DefaultQueue");
    }

    if (! std::is_detected_v<iterator_t, CopyFastMoveQueue>) {
        error("test_iterator: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_v<iterator_t, DeepCopyQueue>) {
        error("test_iterator: error on DeepCopyQueue");
    }
} 



template<typename T>
using const_iterator_t = typename T::const_iterator;

void test_const_iterator() {
    if (! std::is_detected_v<const_iterator_t, MoveOnlyQueue>) {
        error("test_const_iterator: error on MoveOnlyQueue");
    }

    if (! std::is_detected_v<const_iterator_t, DefaultQueue>) {
        error("test_const_iterator: error on DefaultQueue");
    }

    if (! std::is_detected_v<const_iterator_t, CopyFastMoveQueue>) {
        error("test_const_iterator: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_v<const_iterator_t, DeepCopyQueue>) {
        error("test_const_iterator: error on DeepCopyQueue");
    }
} 



template<typename T>
using reverse_iterator_t = typename T::reverse_iterator;

void test_reverse_iterator() {
    if (! std::is_detected_v<reverse_iterator_t, MoveOnlyQueue>) {
        error("test_reverse_iterator: error on MoveOnlyQueue");
    }

    if (! std::is_detected_v<reverse_iterator_t, DefaultQueue>) {
        error("test_reverse_iterator: error on DefaultQueue");
    }

    if (! std::is_detected_v<reverse_iterator_t, CopyFastMoveQueue>) {
        error("test_reverse_iterator: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_v<reverse_iterator_t, DeepCopyQueue>) {
        error("test_reverse_iterator: error on DeepCopyQueue");
    }
} 



template<typename T>
using const_reverse_iterator_t = typename T::const_reverse_iterator;

void test_const_reverse_iterator() {
    if (! std::is_detected_v<const_reverse_iterator_t, MoveOnlyQueue>) {
        error("test_const_reverse_iterator: error on MoveOnlyQueue");
    }

    if (! std::is_detected_v<const_reverse_iterator_t, DefaultQueue>) {
        error("test_const_reverse_iterator: error on DefaultQueue");
    }

    if (! std::is_detected_v<const_reverse_iterator_t, CopyFastMoveQueue>) {
        error("test_const_reverse_iterator: error on CopyFastMoveQueue");
    }

    if (! std::is_detected_v<const_reverse_iterator_t, DeepCopyQueue>) {
        error("test_const_reverse_iterator: error on DeepCopyQueue");
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
    _test_constructor_default<false, true, MoveOnlyQueue>(
        "test_constructor_default: MoveOnlyQueue compile-time's size is not 0",
        "test_constructor_default: MoveOnlyQueue runtime's size is not 0");

    _test_constructor_default<true, true, DefaultQueue>(
        "test_constructor_default: DefaultQueue compile-time's size is not 0",
        "test_constructor_default: DefaultQueue runtime's size is not 0");

    _test_constructor_default<false, true, CopyFastMoveQueue>(
        "test_constructor_default: CopyFastMoveQueue compile-time's size is not 0",
        "test_constructor_default: CopyFastMoveQueue runtime's size is not 0");

    _test_constructor_default<true, true, DeepCopyQueue>(
        "test_constructor_default: DeepCopyQueue compile-time's size is not 0",
        "test_constructor_default: DeepCopyQueue runtime's size is not 0");
}



void test_constructor_init_default_n() {
    MoveOnlyQueue MoveOnlyQueue_rt(typename MoveOnlyQueue::size_type{5});
    typename MoveOnlyQueue::size_type MoveOnlyQueue_rt_size = MoveOnlyQueue_rt.size();

    if (MoveOnlyQueue_rt_size != 5) {
        error("test_constructor_init_default_n: MoveOnlyQueue_rt's size is not 5");
    }


    constexpr DefaultQueue DefaultQueue_cs(typename DefaultQueue::size_type{5});
    constexpr typename DefaultQueue::size_type DefaultQueue_cs_size = DefaultQueue_cs.size();

    if constexpr (DefaultQueue_cs_size != 5) {
        error("test_constructor_init_default_n: DefaultQueue_cs's size is not 5");
    }

    DefaultQueue DefaultQueue_rt(typename DefaultQueue::size_type{5});
    typename DefaultQueue::size_type DefaultQueue_rt_size = DefaultQueue_rt.size();

    if (DefaultQueue_rt_size != 5) {
        error("test_constructor_init_default_n: DefaultQueue_rt's size is not 5");
    }


    CopyFastMoveQueue CopyFastMoveQueue_rt(typename CopyFastMoveQueue::size_type{5});
    typename CopyFastMoveQueue::size_type CopyFastMoveQueue_rt_size = CopyFastMoveQueue_rt.size();

    if (CopyFastMoveQueue_rt_size != 5) {
        error("test_constructor_init_default_n: CopyFastMoveQueue_rt's size is not 5");
    }


    constexpr DeepCopyQueue DeepCopyQueue_cs(typename DeepCopyQueue::size_type{5});
    constexpr typename DeepCopyQueue::size_type DeepCopyQueue_cs_size = DeepCopyQueue_cs.size();

    if constexpr (DeepCopyQueue_cs_size != 5) {
        error("test_constructor_init_default_n: DeepCopyQueue_cs's size is not 5");
    }

    DeepCopyQueue DeepCopyQueue_rt(typename DeepCopyQueue::size_type{5});
    typename DeepCopyQueue::size_type DeepCopyQueue_rt_size = DeepCopyQueue_rt.size();

    if (DeepCopyQueue_rt_size != 5) {
        error("test_constructor_init_default_n: DeepCopyQueue_rt's size is not 5");
    }
}



void test_constructor_init_copy_n() {
    constexpr DefaultQueueElement DefaultQueue_cs_element{ 0 };
    constexpr DefaultQueue DefaultQueue_cs(typename DefaultQueue::size_type{5}, DefaultQueue_cs_element);
    constexpr typename DefaultQueue::size_type DefaultQueue_cs_size = DefaultQueue_cs.size();

    if constexpr (DefaultQueue_cs_size != 5 && DefaultQueue_cs_element == DefaultQueue_cs[0] && DefaultQueue_cs_element == DefaultQueue_cs[4]) {
        error("test_constructor_init_default_n: DefaultQueue_cs's size is not 5, or the copy did something wrong");
    }

    DefaultQueueElement DefaultQueue_rt_element{ 0 };
    DefaultQueue DefaultQueue_rt(typename DefaultQueue::size_type{5}, DefaultQueue_rt_element);
    typename DefaultQueue::size_type DefaultQueue_rt_size = DefaultQueue_rt.size();

    if (DefaultQueue_rt_size != 5 && DefaultQueue_rt_element == DefaultQueue_rt[0] && DefaultQueue_rt_element == DefaultQueue_rt[4]) {
        error("test_constructor_init_default_n: DefaultQueue_rt's size is not 5, or the copy did something wrong");
    }


    CopyFastMoveQueueElement CopyFastMoveQueue_rt_element{ 0, 1, 2 };
    CopyFastMoveQueue CopyFastMoveQueue_rt(typename CopyFastMoveQueue::size_type{5}, CopyFastMoveQueue_rt_element);
    typename CopyFastMoveQueue::size_type CopyFastMoveQueue_rt_size = CopyFastMoveQueue_rt.size();

    if (CopyFastMoveQueue_rt_size != 5 && CopyFastMoveQueue_rt_element == CopyFastMoveQueue_rt[0] && CopyFastMoveQueue_rt_element == CopyFastMoveQueue_rt[4]) {
        error("test_constructor_init_default_n: CopyFastMoveQueue_rt's size is not 5, or the copy did something wrong");
    }


    constexpr DeepCopyQueueElement DeepCopyQueue_cs_element{ 0, 1, 2 };
    constexpr DeepCopyQueue DeepCopyQueue_cs(typename DeepCopyQueue::size_type{5}, DeepCopyQueue_cs_element);
    constexpr typename DeepCopyQueue::size_type DeepCopyQueue_cs_size = DeepCopyQueue_cs.size();

    if constexpr (DeepCopyQueue_cs_size != 5 && DeepCopyQueue_cs_element == DeepCopyQueue_cs[0] && DeepCopyQueue_cs_element == DeepCopyQueue_cs[4]) {
        error("test_constructor_init_default_n: DeepCopyQueue_cs's size is not 5, or the copy did something wrong");
    }

    DeepCopyQueueElement DeepCopyQueue_rt_element{ 0, 1, 2 };
    DeepCopyQueue DeepCopyQueue_rt(typename DeepCopyQueue::size_type{5});
    typename DeepCopyQueue::size_type DeepCopyQueue_rt_size = DeepCopyQueue_rt.size();

    if (DeepCopyQueue_rt_size != 5 && DeepCopyQueue_rt_element == DeepCopyQueue_rt[0] && DeepCopyQueue_rt_element == DeepCopyQueue_rt[4]) {
        error("test_constructor_init_default_n: DeepCopyQueue_rt's size is not 5, or the copy did something wrong");
    }
}



int main() {
    using func_t = void(*)();

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

        // Special members

        // Capacity

        // Element Access

        // Modifier

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

    for(auto const& m : error_messages) {
        std::cerr << m << '\n';
    }

    return error_count;
}