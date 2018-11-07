# **haz::SharedQueue::clear**

```cpp
constexpr void clear() noexcept;
```

Remove all elements of the container. After that `size()` returns 0.
Invalidate all iterators, references and pointers.

## Parameters
(none)

## Return value
(none)

## Complexity
Constant if the elements are trivially destructible, otherwise linear in the size of the container.

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2 };
    std::cout << "Size before : " << queue.size() << '\n';
    queue.clear();
    std::cout << "Size after : " << queue.size() << '\n';
}
```
Output:
```
Size before : 3
Size afer : 0
```

## See also
|||
| ---:| --- |
| [pop_front](pop_front.md) | Pop the first element |
