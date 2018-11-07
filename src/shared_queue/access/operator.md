# **haz::SharedQueue::operator[]**

```cpp
constexpr const_reference operator[](size_type index) const noexcept;
constexpr reference operator[](size_type index) noexcept;
```

Returns the number a reference to the element at the index specified.

## Parameters
|||
| ---:| --- |
| index | index of the element to return |

## Return value
Reference to the requested element.

## Complexity
Constant.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2 };
    std::cout << "The second element is " << queue[1] << '\n';
}
```
Output:
```
The second element is 1
```

## See also
|||
| ---:| --- |
| [at](at.md) | Access specified element with bounds checking |
