# **haz::SharedQueue::back**

```cpp
constexpr const_reference back() const noexcept;
constexpr reference back() noexcept;
```

Returns the last element of the container.
Calling `back` on an empty container is undefined.

## Parameters
(none)

## Return value
Reference to the last element.

## Complexity
Constant.

## Notes
The element returned is the last element that has been pushed.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2 };
    std::cout << "The last element is " << queue.back() << '\n';
}
```
Output:
```
The last element is 2
```

## See also
|||
| ---:| --- |
| [front](front.md) | Access the first element |
