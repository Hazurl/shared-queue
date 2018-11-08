# **haz::SharedQueue::swap**

```cpp
constexpr void swap(SharedQueue& other) noexcept(/* see below */);
```

Exchanges the contents of the container with those of other. Does not cause iterators and references to associate with the other container.


## Parameters
|||
|---:|---|
| other | Container to exchange the contents with |

## Return value
(none)

## Exception
> **noexcept specification:**
> `noexcept(is_nothrow_swappable_v<T>)`


## Complexity
Linear in the size of the container.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

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
    queue_a.swap(queue_b);

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
| ---:| --- |
| [clear](clear.md) | Clear the content |
