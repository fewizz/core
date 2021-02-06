#pragma once

#include <cinttypes>
#include <concepts>
#include <cstdint>
#include <stdint.h>

namespace util {

template<uint8_t HeadBit, uint8_t... Bits>
constexpr void check_bits() {
    static_assert(HeadBit <= 1, "bit can't be greater than 1");
    if constexpr(sizeof...(Bits) > 0) check_bits<Bits...>();
}

template<std::integral Int>
constexpr bool equals(Int mask, uint8_t bits, Int val, uint8_t position) {
    Int mask0 = ~(Int(-1) << bits);
    mask0 <<= position;
    return (val & mask0) == (mask << position);
}

template<uint8_t... Bits>
constexpr bool equals(std::integral auto val, uint8_t position) {
    check_bits<Bits...>();
    using Int = decltype(val);

    Int mask = 0;
    for(uint8_t bit : {Bits...})
        mask = (mask << 1) | bit;
    
    return equals(mask, sizeof...(Bits), val, position);
}

template<std::integral Int>
constexpr bool equalsr(Int mask, uint8_t bits, Int val) {
    return equals(mask, bits, val, 0);
}

template<std::integral Int>
constexpr bool equalsl(Int mask, uint8_t bits, Int val) {
    return equals(mask, bits, val, sizeof(Int)*8 - bits);
}

template<uint8_t... Bits>
constexpr bool equalsr(std::integral auto val) {
    return equals<Bits...>(val, 0);
}

template<uint8_t... Bits>
constexpr bool equalsl(std::integral auto val) {
    return equals<Bits...>(val, sizeof(decltype(val))*8 - sizeof...(Bits));
}

template<std::integral Int>
constexpr Int change_endianness(Int val) {
    constexpr unsigned size = sizeof(Int);

    for(unsigned byte = 0; byte < size / 2; byte++) {
        unsigned
            start_bit0 = byte * 8,
            start_bit1 = (size - byte - 1) * 8;

        Int byte_val0 = ( val >> start_bit0 ) & 0xFF;
        Int byte_val1 = ( val >> start_bit1 ) & 0xFF;

        val &= ~(0xFF << start_bit0);
        val &= ~(0xFF << start_bit1);

        val |= byte_val1 << start_bit0;
        val |= byte_val0 << start_bit1;
    }

    return val;
}

}