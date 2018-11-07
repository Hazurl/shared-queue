# **haz::SharedQueue::size**

```cpp
constexpr size_type size() const noexcept;
```

Returns the number of elements in the container.

## Parameters
(none)

## Return value
The number of elements in a `SharedQueue`.

## Complexity
Constant.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2 };
    std::cout << "The queue contains " << queue.size() << " elements\n";
}
```
Output:
```
The queue contains 3 elements
```

## See also
|||
| ---:| --- |
| [empty](empty.md) | Checks whether the container is empty |
| [max_size](max_size.md) | Returns the maximum number of elements |
