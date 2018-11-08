# **std::swap (haz::SharedQueue)**

```cpp
template<typename T, std::size_t N>
void swap(haz::SharedQueue<T, N> const& lhs, haz::SharedQueue<T, N> const& rhs) noexcept(/* see below */);
```

Specialize the `std::swap` algorithm for `haz::SharedQueue`. Swaps the contents of `lhs` and `rhs`. Calls `lhs.swap(rhs)`.

## Parameters
|||
|---:|---|
| lhs, rhs | Containers whose contents to swap |

## Return value
(none)

## Complexity
Constant.

## Exceptions
> **noexcept specification:**
> noexcept(noexcept(lhs.swap(rhs)))

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>
#include <algorithm>

int main() {
    haz::SharedQueue<int, 10> queue_a{ 0, 1, 2, 3 };
    haz::SharedQueue<int, 10> queue_b{ 4, 5, 6 };
    std::cout << "Contents of A:\n";
    for(auto i : queue_a) {
        std::cout << i << ", ";
    }
    std::cout << '\n';
    std::cout << "Contents of B:\n";
    for(auto i : queue_b) {
        std::cout << i << ", ";
    }
    std::cout << '\n';

    std::cout << "Swap\n";
    std::swap(queue_a, queue_b);

    std::cout << "Contents of A:\n";
    for(auto i : queue_a) {
        std::cout << i << ", ";
    }
    std::cout << '\n';
    std::cout << "Contents of B:\n";
    for(auto i : queue_b) {
        std::cout << i << ", ";
    }
    std::cout << '\n';
}
```
Output:
```
Contents of A:
0, 1, 2, 3,
Contents of B:
4, 5, 6,
Swap
Contents of A:
4, 5, 6,
Contents of B:
0, 1, 2, 3,
```

## See also
|||
|---:|---|
|[swap](../modifier/swap.md)| Swaps the contents |