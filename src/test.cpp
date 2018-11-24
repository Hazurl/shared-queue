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

using DeepCopyQueueElement = std::array<int, 5>;
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



int main() {
    using func_t = void(*)();

    constexpr std::array<func_t, 11> funcs {
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
        test_const_reverse_iterator
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