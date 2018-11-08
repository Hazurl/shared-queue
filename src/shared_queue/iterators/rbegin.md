# **haz::SharedQueue::rbegin, haz::SharedQueue::crbegin**

```cpp
constexpr reverse_iterator rbegin() noexcept();
constexpr const_reverse_iterator rbegin() const noexcept();
constexpr const_reverse_iterator crbegin() const noexcept();
```

Returns an iterator to the first element of the reversed container. It corresponds to the last element of the non-reversed container.
If the container is empty, the returned iterator will be equal to `rend()`

## Parameters
(none)

## Return value
Reverse iterator to the first element.

## Complexity
Constant.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2, 3 };
    for(typename haz::SharedQueue<int, 10>::reverse_iterator it = queue.rbegin(); it != queue.rend(); ++it)
        std::cout << *it << ", ";
    std::cout << '\n';
}
```
Output:
```
3, 2, 1, 0,
```

## See also
|||
| ---:| --- |
| [rend, crend](rend.md) | Returns a reverse iterator to the end |
