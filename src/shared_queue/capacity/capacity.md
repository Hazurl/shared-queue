# **haz::SharedQueue::capacity**

```cpp
constexpr size_type capacity() const noexcept;
```

Returns the maximum number of element in the container. 

## Parameters
(none)

## Return value
Maximum number of elements.

## Complexity
Constant.

## Notes
For `haz::SharedQueue<T, N>`, the value returned is `S`.  
It's a synonym of [max_size](max_size.md).

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue;
    std::cout << "The capacity of the queue is " << queue.capacity() << '\n';
}
```
Output:
```
The capacity of the queue is 10
```

## See also
|||
| ---:| --- |
| [size](size.md) | Returns the number of elements |
| [max_size](max_size.md) | Returns the maximum number of elements |
