# **haz::SharedQueue::push_back**

```cpp
void push_back(T const& value);     (1)
void push_back(T&& value);          (2)
```

Insert the element at the end of the container.  
&nbsp;&nbsp;&nbsp;&nbsp;(1) The new element is initialized as a copy of `value`  
&nbsp;&nbsp;&nbsp;&nbsp;(2) `value` is moved into the new element  

All iterators past-the-end, end included, are invalidated. No reference are invalidated.
If the container is full, i.e. `size() == max_size()` then the behaviour is undefined.

## Parameters
|||
| ---:| --- |
| value | the value of the element to append |

### Type requirement
`T` must meet the requirements of *[CopyInsertable](https://en.cppreference.com/w/cpp/named_req/CopyInsertable)* in order to use overload (1).  
`T` must meet the requirements of *[MoveInsertable](https://en.cppreference.com/w/cpp/named_req/MoveInsertable)* in order to use overload (2).

## Return value
(none)

## Complexity
Constant.

## Exception
If an exception is thrown, this function has no effect (strong exception guarantee).

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>
#include <string>

int main() {
    haz::SharedQueue<std::string, 10> queue;
    
    queue.push_back("abc");
    
    std::string s = "def";
    queue.push_back(std::move(s));

    std::cout << "Contents:\n";
    for(auto const& i : queue) {
        std::cout << std::quoted(i) << '\n';
    }
}
```
Output:
```
Contents:
"abc"
"def"
```

## See also
|||
| ---:| --- |
| [emplace_back](emplace_back.md) | Construct an element in-place at the end |
