#include <stdio.h>
#include <stdint.h>
#include "../lib/util.h"

uint16_t float2float(uint8_t op1_1, uint8_t op1_2, uint8_t op2_1, uint8_t op2_2);

int main() {
    size_t testCount = 11;

    // Test inputs for 11 cases (binary representations of floats)
    uint16_t testInput1[] = {
        // 0b0001101000000100, // Test case 0
        0b0100001000000100, // Test case 1
        0b0100101000010000, // Test case 2
        0b0101001000001111, // Test case 3
        0b0001101000000100, // Test case 4
        0b0100001000000000, // Test case 5
        0b0101001000000000, // Test case 6
        0b0110101000001111, // Test case 7
        0b0001101000000100, // Test case 8
        0b0101101000001000, // Test case 9
        0b0100101000100001, // Test case 10
        0b0110101000100111  // Test case 11
    };

    uint16_t testInput2[] = {
        // 0b0001101000000100, // Test case 0
        0b0100001000000100, // Test case 1
        0b0100001000000100, // Test case 2
        0b0000000000100000, // Test case 3
        0b0001111000000100, // Test case 4
        0b0101011000000100, // Test case 5
        0b0101111000000100, // Test case 6
        0b0101001000000000, // Test case 7
        0b0001101000000100, // Test case 8
        0b0101111000000100, // Test case 9
        0b0100001000000100, // Test case 10
        0b0101001000000000  // Test case 11
    };

    uint16_t testOutputs[] = {
        // 0b0001110000100011, // Test case 0
        0b0100011000000100, // Test case 1
        0b0100101110010001, // Test case 2
        0b0101001000001111, // Test case 3
        0b0010000010000011, // Test case 4
        0b0101011000110100, // Test case 5
        0b0101111000110100, // Test case 6
        0b0110101001000111, // Test case 7
        0b0001111000000100, // Test case 8
        0b0101111000000100, // Test case 9
        0b0100101110010001, // Test case 10
        0b0110101001000111  // Test case 11
    };

    size_t passed = 0;

    test16 testCases[testCount];

    for(size_t i = 0; i < 1; i++)
    {
        uint8_t upper1 = (testInput1[i] >> 8) & 0xFF;
        uint8_t lower1 = testInput1[i] & 0xFF;
        uint8_t upper2 = (testInput2[i] >> 8) & 0xFF;
        uint8_t lower2 = testInput2[i] & 0xFF;

        testCases[i].test = float2float(upper1, lower1, upper2, lower2);
        testCases[i].truth = testOutputs[i];

        printf("Test case %d: ", i+1);
        if(testCases[i].test == testCases[i].truth)
        {
            passed++;
            printf("PASS\n");
        }
        else
        {
            printf("FAIL\n");
        }
        printf("Input1: ");
        printBinary16(testInput1[i]);
        printf("\n");
        printf("Input2: ");
        printBinary16(testInput2[i]);
        printf("\n");
        printf("Test: \t  ");
        printBinary16(testCases[i].test);
        printf("\n");
        printf("Expected: ");
        printBinary16(testCases[i].truth);
        printf("\n");

    }
    printf("Passed %d out of %d tests\n", passed, testCount);

    return 0;
}