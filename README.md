# Multithreaded Queue

Consumer/Producer pattern implementation.
Any number of thread can be either a producer or a consumer, respectively pushing and poping to and from the queue in a transparent way.
An additional policy can be used to add a buffer so that the lock become less stressed.

```cpp 
#include <haz/SharedQueue.hpp>
#include <haz/Producer.hpp>
#include <haz/Consumer.hpp>
#include <thread>
#include <atomic>

using data_t = int;

static std::atomic_bool stop{false};
static constexpr std::size_t size_max = 50;

using Prod_t = haz::Producer<data_t /* Queue's type */, size_max /* Queue's size */, haz::Buffered<15> /* Policy */>;
using Cons_t = haz::Consumer<data_t,                    size_max,                    haz::Buffered<10>>;

data_t real_producer() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // fake work
    data_t n = std::rand() % 900 + 100;
    return n;
}

void real_consumer(data_t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // fake work
}

void producer(Prod_t prod) {
    try {
        while(!stop.load()) {
            data_t value = real_producer();
            prod.push(value);
        }
    } catch(std::runtime_error const&) { /* Interrupt */ } 
}

void consumer(Cons_t cons) {
    try {
        while(!stop.load()) {
            data_t value = cons.pop();
            real_consumer(value);
        }
    } catch(std::runtime_error const&) { /* Interrupt */ } 
}

int main() {
    haz::SharedQueue<data_t, size_max> queue;

    std::thread producer_thread(producer, Prod_t{queue});
    std::thread consumer_thread(consumer, Cons_t{queue});

    std::cin.get();
    scout << "Stopping...\n";

    stop.store(true);      // Stop all non-waiting thread
    queue.interrupt_all(); // Stop all waiting thread, a runtime_error will be threw

    producer_thread.join();
    consumer_thread.join();
}
```


# How to use it

## SharedQueue

That's the queue where the temporary elements will be stored.
It acts as a buffer.

```cpp
using Queue = haz::SharedQueue<Type, Size>;
```

The first template parameter is the type stored. The second is the maximum number of element in the queue.

### Capacity

`contexpr std::size_t capacity() const`
To get back the maximum size, e.g. the second template parameter `Size`

`std::size_t size() const`
To get the number of element in the queue at that moment

`bool empty() const`
Equivalent to `size() == 0`

### Element Access

`Type& top()` and `Type const& top() const` 
Returns a reference to the first pushed element. That's the next one to be poped.

### Modifier

`void pop()`
Remove the first pushed element, if you need to copy it, you can use `top()` before `pop()`. 
If the queue is empty, doesn't modify the queue.

`void push(Type const& t)`
Push the element at the end of the queue. 
If the queue is full, doesn't modify the queue.

### Thread Synchronisation

`std::unique_lock<std::mutex> acquire_lock()`
Returns a unique lock on the queue's mutex.
Use that methods to be sure you are the only one to modify the queue at that point, assuming the other threads call this methods too.

`std::unique_lock<std::mutex> wait_not_empty()`
Returns a unique lock on the queue's mutex.
Wait until the queue is not empty, so you can then push.

`std::unique_lock<std::mutex> wait_not_full()`
Returns a unique lock on the queue's mutex.
Wait until the queue is not full, so you can then pop.

`void interrupt_all()`
If any thread is waiting with `wait_not_empty` or `wait_not_full`, they will be interupted.

> Interruption:
> `wait_not_empty` and `wait_not_full` can throw an exception `std::runtime_error` if the `interrupt_all` has been called in another thread.

> Important:
> All functions in *Element Access* and *Modifier* are not synchronized, 
> you need to call by yourself `acquire_lock` or one of the `wait_*` methods to lock the mutex.

## Producer

This class is an interface to push to the queue without worrying about the mutex.

`using MyProducer = Producer<Type, Size, Policy = StandardPolicy>`
`Type` and `Size` referes to the same argument as the one in `SharedQueue<Type, Size>`.
The policy is dicussed below.

`Producer(SharedQueue<T, S>& queue)`
Construct the Producer with the queue, it keep a rerefence to that queue. 
So the queue must be alive until the producer is destroyed.

`void push(T const& value)` 
Push the value in the queue, acquiring the lock first. 
The function will wait untit it get interrupted or if the queue is not full

## Consumer

This class is an interface to pop from the queue without worrying about the mutex.

`using MyConsumer = Consumer<Type, Size, Policy = StandardPolicy>`
`Type` and `Size` referes to the same argument as the one in `SharedQueue<Type, Size>`.
The policy is dicussed below.

`Consumer(SharedQueue<T, S>& queue)`
Construct the Consumer with the queue, it keep a rerefence to that queue. 
So the queue must be alive until the consumer is destroyed.

`T pop()` 
Pop the value from the queue, acquiring the lock first. 
The function will wait until it get interrupted or if the queue is not empty

## Policies

`Producer` and `Consumer` takes an optional third template parameter to change their behaviour.

### Buffered

`Buffered<Size>`
Is a policy to keep a buffer of size `Size`.
For the `Consumer`, it will push to that buffer until it is full then push all of them to the queue.
For the `Producer`, it will pop from that buffer until it is empty, then try to pop as many element as possible from the queue.
It can be useful when you have a lot of threads pushing/poping to the queue at the same time, so the time waiting for the lock is reduced.

## Out

`Out` is a thread-safe of version of `std::cout`. 
You can instantiate an instance of it, then use `operator <<` to push to the buffer, then in the destructor, the buffer is transfered to `std::cout`.

`scout` is a macro that will returns an instance of `Out`, so it can used as
```cpp
scout << "Hello World!\n"; // output "Hello World!\n" to std::cout at the end of the statement
```

Another version `BufferedOut` can `dump` the buffer manually (and in the destructor), you however need `get()` to access the `Out` instance.
```cpp
{
    BufferedOut out;
    out.get() << "Hello";
    out.dump(); // send everything to std::cout
    out.get() << " World\n";
} // out.dump() is called in the destructor
```
