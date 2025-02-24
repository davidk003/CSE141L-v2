#include <stdio.h>
#include <stdint.h>
#include "../lib/util.h"

uint16_t int2float(uint8_t upper, uint8_t lower);

int main() {
    // Test cases based on the file
    size_t testCount = 19;

    uint16_t testInputs[] = {
        0b0000000000000000, // Test case 0
        0b0000000000000001, // Test case 1
        0b0000000000000010, // Test case 2
        0b0000000000000011, // Test case 3
        0b0000000000001100, // Test case 4
        0b0000000000110000, // Test case 5
        0b0100111100000000, // Test case 6
        0b1000111100000000, // Test case 7
        0b0111111100000000, // Test case 8
        0b0011000010000000, // Test case 9
        0b1000100000000000, // Test case 10
        0b1000001000000000, // Test case 11
        0b0000000001010101, // Test case 12
        0b0000010101010000, // Test case 13
        0b0100000010000000, // Test case 14
        0b0000001000101100, // Test case 15
        0b1000000000100000, // Test case 16
        0b0111111111110000, // Test case 17
        0b1111111111000000  // Test case 18
    };

    uint16_t testOutputs[] = {
        0b0000000000000000, 0b0011110000000000, 0b0100000000000000, 0b0100001000000000,
        0b0100101000000000, 0b0101001000000000, 0b0111010011110000, 0b1111011100010000,
        0b0111011111110000, 0b0111001000010000, 0b1111011110000000, 0b1111011111100000,
        0b0101010101010000, 0b0110010101010000, 0b0111010000001000, 0b0110000001011000,
        0b1111011111111110, 0b0111011111111111, 0b1101010000000000
    };

    test16 testCases[testCount];
    int passed = 0;
    for(size_t i = 0; i < testCount; i++)
    {
        uint8_t upper = (testInputs[i] >> 8) & 0xFF;
        uint8_t lower = testInputs[i] & 0xFF;
        testCases[i].test = int2float(upper, lower);
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
        printBinary16(testCases[i].test);
        printf("\n");
        printBinary16(testCases[i].truth);
        printf("\n");
        printf("%d tests passed\n", passed);
    }


    return 0;
}