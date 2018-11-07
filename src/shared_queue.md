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