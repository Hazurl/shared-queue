# **haz::SharedQueue::front**

```cpp
constexpr const_reference front() const noexcept;
constexpr reference front() noexcept;
```

Returns the first element of the container.
Calling `front` on an empty container is undefined.

## Parameters
(none)

## Return value
Reference to the first element.

## Complexity
Constant.

## Notes
The element returned is the first element that has been pushed but not poped yet.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2 };
    std::cout << "The first element is " << queue.front() << '\n';
}
```
Output:
```
The first element is 0
```

## See also
|||
| ---:| --- |
| [back](back.md) | Access the last element |
