# **haz::SharedQueue::max_size**

```cpp
constexpr size_type max_size() const noexcept;
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
It's a synonym of [capacity](capacity.md).

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue;
    std::cout << "The maximum size of the queue is " << queue.max_size() << '\n';
}
```
Output:
```
The maximum size of the queue is 10
```

## See also
|||
| ---:| --- |
| [size](size.md) | Returns the number of elements |
| [capacity](capacity.md) | Returns the maximum number of elements |
