# CGem library

[![Online documentation](https://codedocs.xyz/curoles/cgem.svg)](https://codedocs.xyz/curoles/cgem)

## Allocate variable length array on stack

Macro [stack_array](@ref stack_array) hides an instance of
[Variable-length Automatic Array](https://gcc.gnu.org/onlinedocs/gcc/Variable-Length.html)
and instead exposes managing object of type [raw_ptr_array](@ref cgm::raw_ptr_array).
If compiler does not support VLAA, then dynamic memory allocation is used.

```cpp
TEST (StackArray, Iterators) {
    int sz{10};
    stack_array(int, sz, a);
    a[0] = 0; a[1] = 1; a[2] = a[1];
    std::fill(a.begin(), a.end(), 3);
    std::iota(a.begin(), a.end(), -4);
    int sum{0};
    for (auto elem : a) {
        sum += elem;
    }
    ASSERT_EQ(sum, std::accumulate(a.begin(), a.end(), 0));
}
```
