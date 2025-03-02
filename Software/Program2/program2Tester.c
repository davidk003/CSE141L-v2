#include <stdio.h>
#include <stdint.h>
#include "../lib/util.h"

uint16_t float2int(uint8_t float1, uint8_t float2);

int main() {
    size_t testCount = 25;
    
    uint16_t testInputs[] = {
        0b0000000000000000, // Test case 1
        0b0011110000000000, // Test case 2
        0b0100000000000000, // Test case 3
        0b0100001000000000, // Test case 4
        0b0100000001000000, // Test case 5
        0b0100000001000000, // Test case 6
        0b0100101100000000, // Test case 7
        0b0100101110000000, // Test case 8
        0b0110001100000000, // Test case 9
        0b0110011100000000, // Test case 10
        0b0111011110000000, // Test case 11
        0b0111101110000000, // Test case 12
        0b1000000000000000, // Test case 13
        0b1011110000000000, // Test case 14
        0b1011110100000000, // Test case 15
        0b1100000000000000, // Test case 16
        0b1100001000000000, // Test case 17
        0b1100000001000000, // Test case 18
        0b1100000001000000, // Test case 19
        0b1100101100000000, // Test case 20
        0b1100101110000000, // Test case 21
        0b1110001100000000, // Test case 22
        0b1110011100000000, // Test case 23
        0b1111011110000000, // Test case 24
        0b1111101110000000  // Test case 25
    };


    uint16_t testOutputs[] = {
        0b0000000000000000, // Test case 1
        0b0000000000000001, // Test case 2
        0b0000000000000010, // Test case 3
        0b0000000000000011, // Test case 4
        0b0000000000000010, // Test case 5
        0b0000000000000010, // Test case 6
        0b0000000000001110, // Test case 7
        0b0000000000001111, // Test case 8
        0b0000001110000000, // Test case 9
        0b0000011100000000, // Test case 10
        0b0111100000000000, // Test case 11
        0b0111111111111111, // Test case 12
        0b0000000000000000, // Test case 13
        0b1111111111111111, // Test case 14
        0b1111111111111111, // Test case 15
        0b1111111111111110, // Test case 16
        0b1111111111111101, // Test case 17
        0b1111111111111110, // Test case 18
        0b1111111111111110, // Test case 19
        0b1111111111110010, // Test case 20
        0b1111111111110001, // Test case 21
        0b1111110010000000, // Test case 22
        0b1111100100000000, // Test case 23
        0b1000100000000000, // Test case 24
        0b1000000000000000  // Test case 25
    };


    test16 testCases[testCount];
    size_t passed = 0;
    for(size_t i = 0; i < testCount; i++)
    {
        uint8_t upper = (testInputs[i] >> 8) & 0xFF;
        uint8_t lower = testInputs[i] & 0xFF;
        testCases[i].test = float2int(upper, lower);
        testCases[i].truth = testOutputs[i];

        printf("Test case %d: Input: ", i+1);
        printBinary16(testInputs[i]);
        if(testCases[i].test == testCases[i].truth)
        {
            passed++;
            printf("\tPASS\n");
        }
        else
        {
            printf("\tFAIL\n");
        }
        printf("Test:\t  ");
        printBinary16(testCases[i].test);
        printf("\n");
        printf("Expected: ");
        printBinary16(testCases[i].truth);
        printf("\n");
        printf("Passed %d out of %d tests\n", passed, i+1);

    }

    return 0;
}