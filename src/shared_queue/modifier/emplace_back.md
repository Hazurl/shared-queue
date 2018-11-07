# **haz::SharedQueue::emplace_back**

```cpp
template<typename...Args>
reference emplace_back(Args&&... args);
```

Construct a new element at the end of the container. The element is constructed in place using placement-new. The arguments `args...` are forwarded to the constructor using `std::forward<Args>(args)...`.
All iterators past-the-end, end included, are invalidated. No reference are invalidated.
If the container is full, i.e. `size() == max_size()` then the behaviour is undefined.

## Parameters
|||
| ---:| --- |
| args | arguments to forward to the constructor of the element |

### Type requirement
T (the container's element type) must meet the requirement of *[EmplaceConstructible](https://en.cppreference.com/w/cpp/named_req/EmplaceConstructible)*.

## Return value
Reference to the inserted element.

## Complexity
Constant.

## Exception
If an exception is thrown, this function has no effect (strong exception guarantee).

## Example

```cpp
#include <haz/SharedQueue.hpp>
#include <iostream>
#include <string>

struct Person {
    std::string name;
    unsigned age;

    Person(std::string name, unsigned age) : name(name), age(age) {
        std::cout << "A person is constructed!\n";
    }

    Person(Person&& other) : name(std::move(other.name)), age(other.age) {
        std::cout << "A person is moved!\n";
    }

    Person& operator=(Person const& other) = default;
};

int main() {
    haz::SharedQueue<Person, 10> queue;

    std::cout << "emplace_back:\n";
    queue.emplace_back("John", 32);

    std::cout << "\npush_back:\n";
    queue.push_back(Person("Bob", 47));

    std::cout << "\nContents:\n";
    for(auto const& person : queue) {
        std::cout << person.name << " is " << person.age << '\n';
    }
}
```
Output:
```
emplace_back: 
A person is constructed!

push_back: 
A person is constructed!
A person is moved!

Contents:
John is 32
Bob is 47
```

## See also
|||
| ---:| --- |
| [push_back](push_back.md) | Insert an element to the end |
