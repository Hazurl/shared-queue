# **operator==, !=, <, <=, >, >= (haz::SharedQueue)**

```cpp
template<typename T, std::size_t N>
constexpr bool operator==(haz::SharedQueue<T, N> const& lhs, haz::SharedQueue<T, N> const& rhs) noexcept();         (1)
template<typename T, std::size_t N>
constexpr bool operator!=(haz::SharedQueue<T, N> const& lhs, haz::SharedQueue<T, N> const& rhs) noexcept();         (2)
template<typename T, std::size_t N>
constexpr bool operator<(haz::SharedQueue<T, N> const& lhs, haz::SharedQueue<T, N> const& rhs) noexcept();         (3)
template<typename T, std::size_t N>
constexpr bool operator<=(haz::SharedQueue<T, N> const& lhs, haz::SharedQueue<T, N> const& rhs) noexcept();         (4)
template<typename T, std::size_t N>
constexpr bool operator>(haz::SharedQueue<T, N> const& lhs, haz::SharedQueue<T, N> const& rhs) noexcept();         (5)
template<typename T, std::size_t N>
constexpr bool operator>=(haz::SharedQueue<T, N> const& lhs, haz::SharedQueue<T, N> const& rhs) noexcept();         (6)
```

Compare the contents of two queue.
&nbsp;&nbsp;&nbsp;&nbsp;(1-2) Checks if the contents of `lhs` and `rhs` are equal, that is, whether each element in `lhs` compares equals with the element in `rhs` at the same position.
&nbsp;&nbsp;&nbsp;&nbsp;(3-6) Compares the contents of `lhs` and `rhs` lexicographically. The comparaison is performed by a function equivalent to `std::lexicographical_compare`.

## Parameters
|||
|---:|---|
| lhs, rhs | Containers whose contents to compare |

### Type requirement
`T` must meet the requirements of *[EqualityComparable](https://en.cppreference.com/w/cpp/named_req/EqualityComparable)* in order to use overload (1-2).  
`T` must meet the requirements of *[LessThanComparable](https://en.cppreference.com/w/cpp/named_req/LessThanComparable)* in order to use overload (3-6).  

## Return value
(1) `true` if the contents of the containers are equal, `false` otherwise.  
(2) `true` if the contents of the containers are not equal, `false` otherwise.  
(3) `true` if the contents of the `lhs` are lexicographically *less* than the contents of `rhs`, `false` otherwise.  
(4) `true` if the contents of the `lhs` are lexicographically *less than or equal* the contents of `rhs`, `false` otherwise.  
(5) `true` if the contents of the `lhs` are lexicographically *greater* than the contents of `rhs`, `false` otherwise.  
(6) `true` if the contents of the `lhs` are lexicographically *greater than or equal* the contents of `rhs`, `false` otherwise.

## Complexity
Constant.
