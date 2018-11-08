# **haz::SharedQueue::end, haz::SharedQueue::cend**

```cpp
constexpr iterator end() noexcept();
constexpr const_iterator end() const noexcept();
constexpr const_iterator cend() const noexcept();
```

Returns an iterator to the element following the last element of the container. This element acts as a placeholder; attempting to access it results in undefined behaviour, it is not dereferencable. 

## Parameters
(none)

## Return value
Iterator to the element following the last element.

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
| [begin, cbegin](begin.md) | Returns an iterator to the beginning |
