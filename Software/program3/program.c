#include <stdint.h>
#include <stdio.h>
#include "../lib/util.h"

uint16_t concatFixed(uint8_t float1, uint8_t float2)
{
    return (float1 << 8) | float2;
}

// 1 bit sign, 10 bit mantissa, 5 bit exponent
uint16_t float2float(uint8_t op1_1, uint8_t op1_2, uint8_t op2_1, uint8_t op2_2)
{
    //Store in LUTs
    const uint8_t SIGN_MASK = 0b10000000;
    const uint8_t EXP_MASK = 0b01111100;
    const uint8_t MANT_MASK_HIGH = 0b00000011;
    const uint8_t MANT_MASK_LOW = 0b11111111;

    uint8_t exp_diff;
    uint8_t sub_shift;

    uint8_t mantissa1_HIGH = op1_1 & MANT_MASK_HIGH;
    uint8_t mantissa1_LOW = op1_2 & MANT_MASK_LOW;
    uint8_t mantissa2_HIGH = op2_1 & MANT_MASK_HIGH;
    uint8_t mantissa2_LOW = op2_2 & MANT_MASK_LOW;

    uint8_t mantissa3_HIGH;
    uint8_t mantissa3_LOW;

    uint8_t exp1 = op1_1 & EXP_MASK;
    uint8_t exp2 = op2_1 & EXP_MASK;
    uint8_t ZERO_EXP_TRAP = 0b00000000;
    if(exp1 != ZERO_EXP_TRAP)
    {
        mantissa1_HIGH = mantissa1_HIGH | 0b000000100;
    }
    if(exp2 != ZERO_EXP_TRAP)
    {
        mantissa2_HIGH = mantissa2_HIGH | 0b000000100;
    }

    //Default output exp as exp1, if exp2>exp1, set output exp as exp2.
    uint8_t exp3 = exp1;


    if (exp2 > exp1)
    {

        exp3 = exp2;
        exp_diff = exp2 - exp1;
        exp_diff = exp_diff >> 2;
        sub_shift = exp_diff - 8;
        //Mantissa1 shift right by exp_diff
        if (exp_diff >= 8)
        {
            mantissa1_LOW = mantissa1_LOW >> sub_shift;
            mantissa1_HIGH = 0b00000000;
        }
        else
        {
            uint8_t mask = (1 << exp_diff) - 1; // Creates a mask of 1s, length exp_diff.
            uint8_t carry = mantissa1_HIGH & mask;
            carry = carry << (8 - exp_diff);    // Shift carry to the upper part of mantissa2_LOW.
            mantissa1_HIGH = mantissa1_HIGH >> exp_diff;
            mantissa1_LOW = (mantissa1_LOW >> exp_diff) | carry;
        }

    }
    else
    {
        exp_diff = exp1 - exp2;
        exp_diff = exp_diff >> 2;
        sub_shift = exp_diff - 8;
        //Mantissa2 shift right by exp_diff
        if (exp_diff >= 8)
        {
            mantissa2_LOW = mantissa2_LOW >> sub_shift;
            mantissa2_HIGH = 0b00000000;
        }
        else
        {
            uint8_t mask = (1 << exp_diff) - 1; // Creates a mask of 1s, length exp_diff.
            uint8_t carry = mantissa2_HIGH & mask;
            carry = carry << (8 - exp_diff);    // Shift carry to the upper part of mantissa2_LOW.
            mantissa2_HIGH = mantissa2_HIGH >> exp_diff;
            mantissa2_LOW = (mantissa2_LOW >> exp_diff) | carry;
        }

    }

    // mantissa3 = mantissa1 + mantissa2
    mantissa3_LOW = mantissa1_LOW + mantissa2_LOW;
    uint8_t carry = 0;
    if(mantissa3_LOW < mantissa1_LOW)
    {
        carry = 1;
    }
    if(mantissa3_LOW < mantissa2_LOW)
    {
        carry = 1;
    }
    mantissa3_HIGH = carry;
    mantissa3_HIGH = mantissa3_HIGH + mantissa1_HIGH;
    mantissa3_HIGH = mantissa3_HIGH + mantissa2_HIGH;


    uint8_t CHECK_MANTISSA_OVERFLOW = 0b00001000; //If 12th bit is occupied then overflow
    uint8_t CHECK = mantissa3_HIGH & CHECK_MANTISSA_OVERFLOW;
    if(CHECK == CHECK_MANTISSA_OVERFLOW)
    {
        exp3 = exp3 >> 2;
        exp3 = exp3 + 1;
        exp3 = exp3 << 2;
        uint8_t carry = mantissa3_HIGH & 0b00000001;
        carry = carry << 7;
        
        mantissa3_HIGH = mantissa3_HIGH >> 1;

        mantissa3_LOW = mantissa3_LOW >> 1;
        mantissa3_LOW = mantissa3_LOW | carry;
    }
    

    uint8_t res_sign = op1_1 & SIGN_MASK; // No sub case so just assign sign
    exp3 = exp3 & EXP_MASK;
    uint8_t fixed1 = res_sign | exp3;
    //Mask off 10 bits from mantissa3 MANT_MASK_HIGH=0b00000011
    mantissa3_HIGH = mantissa3_HIGH & MANT_MASK_HIGH;
    printf("mantissa3: ");
    printBinary8(mantissa3_HIGH);
    printf("_");
    printBinary8(mantissa3_LOW);
    printf("\n");
    fixed1 = fixed1 | mantissa3_HIGH;
    uint8_t fixed2 = mantissa3_LOW;

    return concatFixed(fixed1, fixed2);


}
