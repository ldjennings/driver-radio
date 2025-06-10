// #include "main/typedefs.h"
#include "cstdint"
#include "stdio.h"
#include "bitset"

#include <cstdint>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define nibble 0b111
#define input 0b0000'0011'1000'1100
#define example 0x0A11

static u16 extract_bits(u16 value, u8 start, u8 end) {
    end++;
    if (start >= end || end > 16) return 0;
    u16 mask = ((1U << (end - start)) - 1U) << start;
    return (value & mask) >> start;
}

static u16 set_bits(u16 original, uint8_t start, uint8_t end, u16 new_value) {
    end++;
    if (start >= end || end > 16) return original;
    u16 mask = ((1U << (end - start)) - 1U) << start;
    original &= ~mask; // Clear the bits
    original |= (new_value << start) & mask; // Set new bits
    return original;
}


int main() {

    u16 in = 0x006c;

    // in = set_bits(in, 9, 9, 0x1);
    // in = set_bits(in, 3, 3, 0x0);
    // in = set_bits(in, 2, 2, 0x0);



    
    printf("New Number: 0x%04X\n", in);
    printf("Binary: 0b%s\n", std::bitset<16>(in).to_string().c_str());
    printf("Indecies: FEDCBA9876543210\n");

    // u16 out = set_bits(example, 8, 15, 0xde);

    // printf("First 4 bytes: 0x%02X\n", extract_bits(out, 8, 15));
    // printf("Overall message: 0x%04X ", out);

    return 0;
    
}