# Shared Queue

*Defined in header <haz/SharedQueue.hpp>*
```cpp
template<
    class T,
    std::size_t N
> class SharedQueue;
```

`SharedQueue` is an fixed size queue with additional functions to controle multiple access at the same time. The elements are stored contiguously in a buffer of size `N`, but are not in order. The iterators will iterate through the collection in order of insertion though.

Complexity:
    - Insertion at the beginning and removal at the end of elements: constant *O(1)*
    - Random Access: constant *O(1)*

## Template parameters

> T: The type of the elements
> N: The maximum number of element in the buffer

**It is recommended to use a power of 2 for the size `S` so the index is computed faster**.

## Members functions

### Element access
|||
| ---:| --- |
| [at](shared_queue/access/at.md) | Access specified element with bounds checking |
| [back](shared_queue/access/back.md) | Access the last element |
| [front](shared_queue/access/front.md) | Access the first element |
| [operator []](shared_queue/access/operator.md) | Access specified element without bounds checking |

### Iterators
|||
| ---:| --- |
| [begin, cbegin](shared_queue/iterators/begin.md) | Returns an iterator to the beginning |
| [end, cend](shared_queue/iterators/end.md) | Returns an iterator to the end |
| [rbegin, crbegin](shared_queue/iterators/rbegin.md) | Returns a reverse iterator to the beginning |
| [end, cend](shared_queue/iterators/end.md) | Returns a reverse iterator to the end |

### Capacity
|||
| ---:| --- |
| [capacity](shared_queue/capacity/capacity.md) | Returns the maximum number of elements |
| [empty](shared_queue/capacity/empty.md) | Checks whether the container is empty |
| [max_size](shared_queue/capacity/max_size.md) | Returns the maximum number of elements |
| [size](shared_queue/capacity/size.md) | Returns the number of elements |

### Modifiers
|||
| ---:| --- |
| [assign](modifier/assign.md) | Replaces the content |
| [clear](shared_queue/modifier/clear.md) | Clear the content |
| [emplace_back](shared_queue/modifier/emplace_back.md) | Construct an element in-place at the end |
| [pop_front](shared_queue/modifier/pop_front.md) | Pop the first element |
| [push_back](shared_queue/modifier/push_back.md) | Insert an element to the end |
| [swap](shared_queue/modifier/swap.md) | Swaps the contents |

### Non-member function
|||
| ---:| --- |
| [operator ==, !=, <=, >=, <, >](shared_queue/non_member/compare.md) | Lexicograpically compares the values |
| [std::swap](shared_queue/non_member/swap.md) | specialize the `std::swap` algorithm |
