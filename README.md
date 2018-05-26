# CGem library

## Allocate variable size array on stack

```cpp
TEST (StackArray, Iterators) {
    stack_array(int, 10, a);
    std::fill(a.begin(), a.end(), 3);
    std::iota(a.begin(), a.end(), -4);
    int sum{0};
    for (auto elem : a) {
        sum += elem;
    }
    ASSERT_EQ(sum, std::accumulate(a.begin(), a.end(), 0));
}
```
