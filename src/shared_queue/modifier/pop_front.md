# **haz::SharedQueue::pop_front**

```cpp
constexpr void pop_front() noexcept;
```

Removes the first element of the container. If there are no elements in the container, the behaviour is undefined.
Iterator and reference to the erased element are invalidated. The other are not unless the container is now empty.

## Parameters
(none)

## Return value
(none)

## Complexity
Constant.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2, 3 };
    std::cout << "Contents:\n";
    for(auto i : queue) {
        std::cout << i << '\n';
    }

    std::cout << "Pop 2 values\n";
    queue.pop_front();
    queue.pop_front();

    std::cout << "Contents:\n";
    for(auto i : queue) {
        std::cout << i << ", ";
    }
    std::cout << '\n';
}
```
Output:
```
Contents:
0, 1, 2, 3,
Pop 2 values
Contents:
2, 3,
```

## See also
|||
| ---:| --- |
| [clear](clear.md) | Clear the content |
