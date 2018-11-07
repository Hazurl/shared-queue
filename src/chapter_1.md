# Chapter 1

```cpp
#include <iostream>
int main() {
    auto a = 1;
    int i = a;
    ++i;
    auto f = [] (int j) { return j + 2; };
    i = f(i*a);
    std::cout << i << '\n';
}
```