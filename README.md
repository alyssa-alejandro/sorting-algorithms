# Sorting algorithms — Question 3(b)

## Why comparison sorting has an Ω(n log n) lower bound

The claim needs a qualification: the lower bound applies to **comparison-based** sorting of arbitrary keys when the only information the algorithm obtains about keys is through pairwise comparisons. It is not a lower bound for every possible sorting algorithm or every restricted input domain.

For `n` distinct keys, there are `n!` possible input orders. Model a comparison sort as a binary decision tree: each internal node asks a comparison such as `a[i] < a[j]`, and each outgoing edge represents one of its two outcomes. At a leaf, the algorithm has learned enough to return the correct sorted order. Two different input permutations cannot reach the same leaf, since the algorithm would then return the same order for both and be wrong on one. Therefore the tree has at least `n!` leaves.

A binary tree of height `h` has at most `2^h` leaves. Consequently,

```text
2^h >= n!       =>       h >= log2(n!)
```

Using Stirling's approximation, `log2(n!) = n log2(n) - Θ(n)`, so `h = Ω(n log n)`. Since each comparison takes constant time in the comparison model, this is also a worst-case time lower bound for comparison sorts. Merge sort and heap sort take `O(n log n)`, so they are asymptotically optimal within this model. The lower bound does not say every input takes that long, nor does it rule out an algorithm that uses information about key representation or a restricted key range.

## Beating the bound with radix sort

This project implements stable least-significant-digit (LSD) radix sort for signed 32-bit integers. Each byte is sorted in turn using a stable counting-sort pass. Four passes process every key, and each pass costs `O(n + 256)`, so for fixed-width 32-bit keys the total is `O(4(n + 256)) = O(n)` time, with `O(n + 256)` auxiliary space. This is asymptotically better than `O(n log n)` as `n` grows.

There is no contradiction with the proof: radix sort examines bytes/digits of the integer representation rather than discovering the order solely from pairwise comparisons. The bound is also conditional on the key width being fixed. If the number of digits is `d` and the radix is `b`, the usual bound is `O(d(n + b))`; when `d` grows with the input, the runtime is not necessarily linear. The implementation handles the entire `int32_t` range, including negative values, by flipping the sign bit before extracting bytes. This maps signed numeric order to unsigned lexicographic byte order.

## Build and run

With a C++17 compiler:

```sh
g++ -std=c++17 -Wall -Wextra -pedantic radix_sort.cpp -o radix_sort
./radix_sort
```

The built-in example includes negative values, duplicates, zero, and the signed 32-bit limits. It prints the input and sorted output, then checks that the result is ordered and has the same elements as the input.