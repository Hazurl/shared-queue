# **haz::SharedQueue::clear**

```cpp
template<typename InputIt>
constexpr void assign(InputIt first, InputIt last) noexcept(/* see below */);           (1)
constexpr void assign(size_type n, T const& value) noexcept(/* see below */);           (2)
constexpr void assign(std::initilializer_list<T> list) noexcept(/* see below */);       (3)
```

Replaces the content of the container.  
&nbsp;&nbsp;&nbsp;&nbsp;(1) Copy each element in the range [`first`, `last`) to the container  
&nbsp;&nbsp;&nbsp;&nbsp;(2) Replaces the content with `n` copies of `value`  
&nbsp;&nbsp;&nbsp;&nbsp;(3) Replaces the content with those in the `list`  

## Parameters
|||
|---:|---|
| n | the new size of the container |
| value | the value to initialize the elements with |
| first, last | the range to copy the elements from |
| list | initiliazer list to copy from |

## Return value
(none)

## Exception
> **noexcept specification:**
> `noexcept(is_nothrow_constructible_v<decltype(*std::declval<InputIt>)>)` for (1)  
> `noexcept(is_nothrow_copy_constructible_v<T>)` for (2-3)  

## Complexity
&nbsp;&nbsp;&nbsp;&nbsp;(1) Linear in distance between `first` and `last`  
&nbsp;&nbsp;&nbsp;&nbsp;(2) Linear in `n`  
&nbsp;&nbsp;&nbsp;&nbsp;(3) Linear in `list.size()`  

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>

int main() {
    haz::SharedQueue<int, 10> queue{ 0, 1, 2 };

    std::cout << "Contents:\n";
    for(auto i : queue) {
        std::cout << i << ", ";
    }
    std::cout << '\n';

    queue.assign(5, 1337);

    std::cout << "Contents:\n";
    for(auto i : queue) {
        std::cout << i << ", ";
    }
    std::cout << '\n';
}
```
Output:
```
Contents:
0, 1, 2,
Contents:
1337, 1337, 1337, 1337, 1337,
```

## See also
|||
| ---:| --- |
| [(constructor)](../special/constructor.md) | Constructs the SharedQueue |
