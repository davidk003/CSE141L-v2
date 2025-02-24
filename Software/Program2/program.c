#include <stdint.h>
//Program 2: floatToFixed
uint16_t concatFixed(uint8_t fixed1, uint8_t fixed2)
{
    return (fixed1 << 8) | fixed2;
}

uint16_t float2int(uint8_t float1, uint8_t float2)
{
    uint8_t EXP_MASK = 0b01111100;
    uint8_t MSB_MASK = 0b10000000;
    uint8_t ALL_ONES = 0b11111111;
    uint8_t ZERO = 0b00000000;

    uint8_t fixed1 = ZERO;
    uint8_t fixed2 = ZERO;
    uint8_t INF_MASK = 0b11111100;
    // If the exponent indicates overflow (exp[4:1] all 1s):

    uint8_t CHECK_INF = float1 & INF_MASK;
    uint8_t NEG_INF = 0b11111100;
    if (CHECK_INF == NEG_INF) // Trigger if exp is 0b01111000 or 0b01111100
    {
        fixed1 = 0b10000000;
        fixed2 = ZERO;
        return concatFixed(fixed1, fixed2);
    }
    uint8_t INF = 0b01111100;
    if (CHECK_INF == INF) // Trigger if exp is 0b01111000 or 0b01111100
    {
        fixed1 = 0b01111111;
        fixed2 = 0b11111111;
        return concatFixed(fixed1, fixed2);
    }

    uint8_t frac_39_32 = ZERO;
    uint8_t frac_31_24 = ZERO;
    uint8_t frac_23_16 = ZERO;
    uint8_t frac_15_8  = ZERO;
    uint8_t frac_7_0   = ZERO;
    
    // Note that int_frac is 40 bits long, first 29 bits are zeros, the or of all exp bits, then last 10 bits of float
    // I wonder if we can simulate the first 29 bits because its all the same anyway
    // int_frac = {29'b0,|flt_in[14:10],flt_in[ 9: 0]}; in SV
    // uint8_t int_frac_1 = 0b00000000; // Not actually required to implement cause shift is identical for frac 1-3
    // uint8_t int_frac_2 = 0b00000000;
    // uint8_t int_frac_3 = 0b00000000;
    // uint8_t int_frac_4 = 0b00000000; // First 5 bits are 0, 3rd to last bit is OR reduction of exponent, last 2 bits are from float
    // uint8_t int_frac_5 = float2;
    
    // OR reduction of exp, essentially compare with all 0s otherwise its 1.
    // (handled this above in a trap, OR reduction = 1 here)
    uint8_t EXP = float1 & EXP_MASK;
    if(EXP != ZERO) //If exp is 0, OR reduction is 0
    {
        uint8_t ORREDUCE = 0b00000100;
        frac_15_8 = frac_15_8 | ORREDUCE;
    }
    uint8_t float9_8 = 0b00000011;
    frac_15_8 = float1 | float9_8;
    frac_7_0 = float2;
    
    EXP = float1 & EXP_MASK;
    // EXP is 0XXXXX00, so shift right 2 to get rid of the 00
    EXP = EXP >> 2;
    // Shift int_frac39 to 0 left by the value of exp.

    uint8_t t = EXP >> 3; // Number of full byte shifts (0 to 5)
    uint8_t s = EXP & 7;  // Basically get remainder of EXP / 8

    // Handle full byte shifts
    if (t == 0) {
        // No full byte shift
    } else if (t == 1) {
        frac_39_32 = frac_31_24;
        frac_31_24 = frac_23_16;
        frac_23_16 = frac_15_8;
        frac_15_8 = frac_7_0;
        frac_7_0 = ZERO;
    } else if (t == 2) {
        frac_39_32 = frac_23_16;
        frac_31_24 = frac_15_8;
        frac_23_16 = frac_7_0;
        frac_15_8 = ZERO;
        frac_7_0 = ZERO;
    } else if (t == 3) {
        frac_39_32 = frac_15_8;
        frac_31_24 = frac_7_0;
        frac_23_16 = ZERO;
        frac_15_8 = ZERO;
        frac_7_0 = ZERO;
    } else if (t == 4) {
        frac_39_32 = frac_7_0;
        frac_31_24 = ZERO;
        frac_23_16 = ZERO;
        frac_15_8 = ZERO;
        frac_7_0 = ZERO;
    } else {
        // t == 5 or greater
        frac_39_32 = ZERO;
        frac_31_24 = ZERO;
        frac_23_16 = ZERO;
        frac_15_8 = ZERO;
        frac_7_0 = ZERO;
    }

    // Handle bit-level shifts
    if (s != 0) {
        t = 8 - s; // Number of bits to shift right for carry-over
        // Shift A
        t = frac_31_24 >> t;
        frac_39_32 = frac_39_32 << s;
        frac_39_32 = frac_39_32 | t;

        // Shift B
        t = 8 - s;
        t = frac_23_16 >> t;
        frac_31_24 = frac_31_24 << s;
        frac_31_24 = frac_31_24 | t;

        // Shift C
        t = 8 - s;
        t = frac_15_8 >> t;
        frac_23_16 = frac_23_16 << s;
        frac_23_16 = frac_23_16 | t;

        // Shift D
        t = 8 - s;
        t = frac_7_0 >> t;
        frac_15_8 = frac_15_8 << s;
        frac_15_8 = frac_15_8 | t;

        // Shift E
        frac_7_0 = frac_7_0 << s;
    }

    uint8_t LAST = 0b00000001;
    uint8_t fixed2MSB = frac_39_32 & LAST;
    fixed1 = frac_39_32 >> 1;
    fixed2 = frac_31_24 >> 1;
    fixed2 = fixed2 | fixed2MSB;
    // If the sign bit is set (negative number):
    uint8_t SIGN = float1 & MSB_MASK;
    if (SIGN == MSB_MASK)
    {
        // return twos complement of output
        // bitwise not of a binary string the same as xor with a string of 1s
        // fixed1 = ~fixed1 + 1;
        // fixed2 = ~fixed2 + 1;
        fixed1 = fixed1 ^ ALL_ONES;
        fixed2 = fixed2 ^ ALL_ONES;
        fixed2 = fixed2 + 1;
        if (fixed2 == ZERO) //If overflow
        {
            fixed1 = fixed1 + 1;
        }
        return concatFixed(fixed1, fixed2);
    }
    else
    {
        // return output
        return concatFixed(fixed1, fixed2);
    }
}

//   Handle special cases for overflow or underflow:
//     If the exponent indicates overflow (exp[4:1] all 1s):
//       If sign bit is set (negative number):
//         return `0x8000` (largest negative)
//       Else:
//         return `0x7FFF` (largest positive)

//  Load the floating-point input:
//     sign = MSB of input
//     exp = Extract bits [14:10] from input
//     int_frac = Concatenate 29 zero bits, the OR of exp bits, and the 10-bit mantissa [9:0] of flt_in NOTE THIS IS 40 BITS (used to be 42 but i cut it down)

//   Adjust integer fraction based on the exponent:
//     Shift int_frac left by the value of exp.

//   Extract fixed-point value:
//     output = Extract bits [39:25] from int_frac

    
//     If the sign bit is set (negative number):
//       return twos complement of output
//     Else:
//       return output