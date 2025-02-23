#include <stdint.h>

uint16_t concatFixed(uint8_t float1, uint8_t float2)
{
    return (float1 << 8) | float2;
}

// 1 bit sign, 10 bit mantissa, 5 bit exponent
uint16_t float2float(uint8_t op1_1, uint8_t op1_2, uint8_t op2_1, uint8_t op2_2)
{
    //Store in LUTs
    uint8_t SIGN_MASK = 0b10000000;
    uint8_t EXP_MASK = 0b01110000;
    uint8_t MANT_MASK = 0b00001111;

    //Will likely have to be stored in memory, not enough registers
    uint8_t exp1 = op1_1 & EXP_MASK;
    uint8_t exp2 = op2_1 & EXP_MASK;
    uint8_t res_sign = op1_1 & SIGN_MASK; // No sub case so just assign sign

    uint8_t mantissa1 = op1_1 & MANT_MASK;
    uint8_t mantissa2 = op2_1 & MANT_MASK;

    uint8_t exp1 = op1_1 & EXP_MASK;
    uint8_t exp2 = op2_1 & EXP_MASK;
    uint8_t res_exp = exp1;
    uint8_t exp_diff = 0;
    if (exp1 < exp2)
    {
        res_exp = exp2;
        exp_diff = exp2 - exp1;
        mantissa1 = mantissa1 >> exp_diff;
    }
    else
    {
        exp_diff = exp1 - exp2;
        mantissa2 = mantissa2 >> exp_diff;
    }

    uint8_t res_mantissa = mantissa1 + mantissa2;
    if (res_mantissa > 15)
    {
        res_mantissa = res_mantissa >> 1;
        res_exp++;
    }

    

}

// // Step 2: Initialize result variables
// sign3 = sign1;  // Assume signs match initially
// exp3 = exp1;    // Default to addend 1's exponent

// // Step 3: Align exponents by right-shifting the smaller mantissa
// if (exp1 > exp2) {
//     exp3 = exp1;
//     mant2 >>= (exp1 - exp2);
// } else if (exp2 > exp1) {
//     exp3 = exp2;
//     mant1 >>= (exp2 - exp1);
// }

// // Step 4: Perform addition
// mant1 = !nil1 ? (mant1 | 0x400) : mant1; // Prepend hidden bit for normalized numbers
// mant2 = !nil2 ? (mant2 | 0x400) : mant2; // Prepend hidden bit for normalized numbers
// mant3 = mant1 + mant2;

// // Step 5: Handle overflow (normalization and rounding)
// if (mant3 & 0x800) {  // Check if most significant bit causes overflow
//     exp3 += 1;        // Increase exponent
//     mant3 >>= 1;      // Normalize mantissa by right-shifting
// }

// // Step 6: Handle rounding-induced overflow
// if (mant3 & 0x800) {  // Re-check after rounding
//     exp3 += 1;
//     mant3 >>= 1;
// }

// // Step 7: Zero-detection for result
// nil3 = (exp3 == 0);

// // Step 8: Store result back to memory
// WriteMemory(132, (sign3 << 7) | (exp3 << 2) | ((mant3 >> 8) & 0x03));
// WriteMemory(133, mant3 & 0xFF);

// // Indicate completion
// done = true;