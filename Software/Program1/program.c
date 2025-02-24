#include <stdint.h>
#include <stdio.h>

uint16_t concatFloat(uint8_t high, uint8_t low) {
    return ((uint16_t)high << 8) | low;
}

uint16_t int2float(uint8_t fixed1, uint8_t fixed2) {
    uint8_t ZERO_CONST = 0x00;
    uint8_t LEADING_ONE_CONST = 0x80;
    uint8_t MIN_EXP = 0xF8; // For negative infinity
    uint8_t ALL_ONE = 0xFF;
    uint8_t BIAS_INITIAL_EXP = 29;

    // Trap cases
    if (fixed2 == ZERO_CONST) {
        if (fixed1 == LEADING_ONE_CONST) {
            // Negative infinity case
            return concatFloat(MIN_EXP, ZERO_CONST);
        }
        if (fixed1 == ZERO_CONST) {
            // Zero
            return 0;
        }
    }

    // Extract sign
    uint8_t sign = fixed1 & LEADING_ONE_CONST;

    // Take absolute value if negative
    uint8_t float1, float2;
    if (sign) {
        // Two’s complement
        float1 = fixed1 ^ ALL_ONE;
        float2 = fixed2 ^ ALL_ONE;
        float2 = float2 + 1;
        if (float2 == 0x00) {
            float1 = float1 + 1;
        }
    } else {
        float1 = fixed1;
        float2 = fixed2;
    }

    // Check if zero after abs
    if (float1 == 0 && float2 == 0) {
        return 0;
    }

    // Normalization
    uint8_t i = 0;
    // Initial shift
    uint8_t temp = (float2 & LEADING_ONE_CONST) >> 7;
    float2 <<= 1;
    float1 = ((float1 << 1) | temp);

    // Shift until leading one found or 15 shifts
    while (i < 15) {
        if (float1 & LEADING_ONE_CONST) {
            break; 
        }
        temp = (float2 & LEADING_ONE_CONST) >> 7;
        float2 <<= 1;
        float1 = ((float1 << 1) | temp);
        i++;
    }

    // If no leading one found (unexpected), treat as zero
    if ((float1 & LEADING_ONE_CONST) == 0) {
        return 0;
    }

    // Remove the implicit leading 1 by shifting once more
    temp = (float2 & LEADING_ONE_CONST) >> 7;
    float2 <<= 1;
    float1 = ((float1 << 1) | temp);

    // Compute exponent
    uint8_t exp = BIAS_INITIAL_EXP - i;

    // Extract mantissa bits
    uint8_t lower6 = (float1 & 0x3F); // 00111111
    lower6 <<= 2; // shift left by 2
    uint8_t top2 = float2 & 0xC0; // 11000000
    float2 = ((top2 >> 6) | lower6);

    // Remove mantissa bits from float1
    float1 >>= 6;

    // Insert exponent (5 bits) into float1
    uint8_t exp_shifted = exp << 2;
    float1 = float1 | exp_shifted;

    // Add sign bit
    float1 = float1 | (sign);

    return concatFloat(float1, float2);
}