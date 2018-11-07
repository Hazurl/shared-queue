# **haz::SharedQueue::empty**

```cpp
constexpr bool empty() const noexcept;
```

Checks whether the container has no elements.

## Parameters
(none)

## Return value
`true` if the element is empty, otherwise `false`.

## Complexity
Constant.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> empty_queue;
    haz::SharedQueue<int, 10> not_empty_queue{ 0, 1, 2 };
    std::cout << "empty_queue is empty ? " << empty_queue.empty() << '\n';
    std::cout << "not_empty_queue is empty ? " << not_empty_queue.empty() << '\n';
}
```
Output:
```
empty_queue is empty ? 1
not_empty_queue is empty ? 0
```

## See also
|||
| ---:| --- |
| [size](size.md) | Returns the number of elements |
