# **haz::SharedQueue::rend, haz::SharedQueue::crend**

```cpp
constexpr reverse_iterator rend() noexcept();
constexpr const_reverse_iterator rend() const noexcept();
constexpr const_reverse_iterator crend() const noexcept();
```

Returns a reverse iterator to the element following the last element of the reversed container. It corresponds to the element preceding the first element of the non-reversed container. This element acts as a placeholder; attempting to access it results in undefined behaviour, it is not dereferencable. 

## Parameters
(none)

## Return value
reverse iterator to the element following the last element.

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
| [rbegin, crbegin](begin.md) | Returns a reverse iterator to the beginning |
