# **haz::SharedQueue::at**

```cpp
constexpr const_reference at(size_type index) const;
constexpr reference at(size_type index);
```

Returns the number a reference to the element at the index specified.
If the index is not within the range [0, size()), an exception of type `std::out_of_range` is thrown.

## Parameters
|||
| ---:| --- |
| index | index of the element to return |

## Return value
Reference to the requested element.

## Exception
`std::out_of_range` if `index >= size()`.

## Complexity
Constant.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2 };
    std::cout << "The second element is " << queue.at(1) << '\n';
}
```
Output:
```
The second element is 1
```

## See also
|||
| ---:| --- |
| [operator []](operator.md) | Access specified element without bounds checking |
