# **haz::SharedQueue::begin, haz::SharedQueue::cbegin**

```cpp
constexpr iterator begin() noexcept();
constexpr const_iterator begin() const noexcept();
constexpr const_iterator cbegin() const noexcept();
```

Returns an iterator to the first element of the container.
If the container is empty, the returned iterator will be equal to `end()`

## Parameters
(none)

## Return value
Iterator to the first element.

## Complexity
Constant.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2, 3 };
    for(typename haz::SharedQueue<int, 10>::iterator it = queue.begin(); it != queue.end(); ++it)
        std::cout << *it << ", ";
    std::cout << '\n';
}
```
Output:
```
0, 1, 2, 3,
```

## See also
|||
| ---:| --- |
| [end, cend](end.md) | Returns an iterator to the end |
