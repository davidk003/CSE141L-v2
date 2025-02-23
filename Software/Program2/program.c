#include <stdint.h>
//Program 2: floatToFixed
uint16_t concatFixed(uint8_t fixed1, uint8_t fixed2)
{
    return (fixed1 << 8) | fixed2;
}

uint16_t float2int(uint8_t float1, uint8_t float2)
{
    uint8_t MAX_NEG_EXP = 0b01111000;
    uint8_t EXP_MASK = 0b01111100;
    uint8_t MSB_MASK = 0b10000000;
    uint8_t ALL_ONES = 0b11111111;
    uint8_t ZERO = 0b00000000;
    uint8_t LEADING_ONE = 0b10000000;
    
    uint8_t fixed1 = ZERO;
    uint8_t fixed2 = ZERO;
    // If the exponent indicates overflow (exp[4:1] all 1s):

    // uint8_t exp = float1 & EXP_MASK;
    // if (exp == ZERO)
    // {
    //     // If exp is 0, then the output is 0
    //     fixed1 = 0b00000000;
    //     fixed2 = 0b00000000;
    //     return concatFixed(fixed1, fixed2);
    // }

    uint8_t exp = float1 & MAX_NEG_EXP;
    if (exp == MAX_NEG_EXP) // Trigger if exp is 0b01111000 or 0b01111100
    {
        // If sign bit is set (negative number):
        uint8_t sign1 = float1 & LEADING_ONE;
        if (sign1 == LEADING_ONE)
        {
            fixed1 = 0b10000000;
            fixed2 = ZERO;
            return concatFixed(fixed1, fixed2);
        }
        else
        {
            fixed1 = 0b01111111;
            fixed2 = 0b11111111;
            return concatFixed(fixed1, fixed2);
        }
    }
    // Note that int_frac is 40 bits long, first 29 bits are zeros, the or of all exp bits, then last 10 bits of float
    //I wonder if we can simulate the first 29 bits because its all the same anyway
    //int_frac = {29'b0,|flt_in[14:10],flt_in[ 9: 0]}; in SV
    // uint8_t int_frac_1 = 0b00000000; //Not actually required to implement cause shift is identical for frac 1-3
    // uint8_t int_frac_2 = 0b00000000;
    // uint8_t int_frac_3 = 0b00000000;
    uint8_t int_frac_4 = 0b00000000; // First 5 bits are 0, 3rd to last bit is OR reduction of exponent, last 2 bits are from float
    uint8_t int_frac_5 = float2;
    
    // OR reduction of exp, essentially compare with all 0s otherwise its 1.
    // (handled this above in a trap, OR reduction = 1 here)
    uint8_t ORREDUCE = 0b00000100;
    uint8_t LAST2 = 0b00000011;
    int_frac_4 = int_frac_4 | ORREDUCE;
    uint8_t UPPERLAST2 = float1 & LAST2;
    int_frac_4 = int_frac_4 | UPPERLAST2;
    int8_t FOURTEEN = 14;
    // Now in_frac4 and 5 contain the reduction bit and the last 10 bits of the float
    if (exp > FOURTEEN)
    {
        exp = exp - FOURTEEN; //Number of times to shift left;
        while (exp > 0)
        {
            fixed1 = fixed1 << 1; //Shift fixed1 left
            uint8_t temp = fixed2 & LEADING_ONE; //Save front of fixed2
            temp = temp >> 7;
            fixed1 = fixed1 | temp; //Add the front of fixed2 to the back of fixed1
            fixed2 = fixed2 << 1; //Shift fixed2 left
            //Shifted fixed1 and fixed2 left.

            //Now shift int_frac_4 and int_frac_5 left
            temp = int_frac_4 & ORREDUCE; //Save the front bit of int_frac_4
            temp = temp >> 2; //Shift it to the back
            fixed2 = fixed2 | temp; //Add the front of int_frac_4 to the back of fixed2
            int_frac_4 = int_frac_4 << 1; //Shift int_frac_4 left
            temp = int_frac_5 & LEADING_ONE; //Save front of int_frac_5
            temp = temp >> 7; //Shift back to the back
            int_frac_4 = int_frac_4 | temp; //Add the front of int_frac_5 to the back of int_frac_4
            int_frac_5 = int_frac_5 << 1; //Shift int_frac_5 left

            exp = exp - 1;
        }
    }
    else
    {
        //Any shift below 14 will not be shifted enough be nonzero.
        fixed1 = ZERO;
        fixed2 = ZERO;
        return concatFixed(fixed1, fixed2);
    }
    
    // If the sign bit is set (negative number):
    uint8_t sign = float1 & MSB_MASK;
    if (sign == MSB_MASK)
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