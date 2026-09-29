#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

// stable counting-sort pass for one byte of each key
static void counting_sort_byte(std::vector<std::int32_t>& values,
                              unsigned int byte_index) {
    constexpr std::size_t radix = 256;
    std::size_t counts[radix] = {};

    const unsigned int shift = byte_index * 8;
    for (std::int32_t value : values) {
        // flip the sign bit so unsigned byte order agrees with signed order
        const std::uint32_t ordered_key =
            static_cast<std::uint32_t>(value) ^ 0x80000000u;
        const std::size_t digit = (ordered_key >> shift) & 0xFFu;
        ++counts[digit];
    }

    // convert frequencies to starting positions in the output array
    std::size_t next_position[radix];
    std::size_t prefix_sum = 0;
    for (std::size_t digit = 0; digit < radix; ++digit) {
        next_position[digit] = prefix_sum;
        prefix_sum += counts[digit];
    }

    std::vector<std::int32_t> output(values.size());
    // forward traversal preserves the order of equal digits
    for (std::int32_t value : values) {
        const std::uint32_t ordered_key =
            static_cast<std::uint32_t>(value) ^ 0x80000000u;
        const std::size_t digit = (ordered_key >> shift) & 0xFFu;
        output[next_position[digit]++] = value;
    }

    values.swap(output);
}

static void radix_sort(std::vector<std::int32_t>& values) {
    constexpr unsigned int bytes_per_key = sizeof(std::int32_t);
    for (unsigned int byte = 0; byte < bytes_per_key; ++byte) {
        counting_sort_byte(values, byte);
    }
}

static void print_values(const char* label,
                         const std::vector<std::int32_t>& values) {
    std::cout << label;
    for (std::int32_t value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}

int main() {
    const std::vector<std::int32_t> input = {
        170, -45, 75, -90, 802, 24, 2, 66, -1, 0, 45, -45,
        INT32_MIN, INT32_MAX
    };
    std::vector<std::int32_t> result = input;

    print_values("Input:  ", input);
    radix_sort(result);
    print_values("Sorted: ", result);

    // demonstration check: sorted order and preservation of every input value
    std::vector<std::int32_t> expected = input;
    std::sort(expected.begin(), expected.end());
    if (!std::is_sorted(result.begin(), result.end()) || result != expected) {
        std::cerr << "Radix sort demonstration check failed.\n";
        return 1;
    }

    std::cout << "Check passed: output is ordered and contains the input values.\n";
    return 0;
}
