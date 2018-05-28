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

## Numeric range for C++11 range-based for loop

```cpp
  for (auto i : num::range(5)) { // [0,5) = 0, 1, 2, 3, 4; the same as range(0, 5)

  for (auto i : num::range(5, 16, 3)) { // 5, 8, 11, 14; with step

  for (auto f : num::range(5.1, 8.3, 0.6)) { // with floating-point numbers

  for (auto i : num::range(16, 5, -3)) { // 16, 13, 10, 7; reversed
```

