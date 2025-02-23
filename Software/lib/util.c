#include <stdint.h>
#include <assert.h>
#include <stdio.h>

void printBinary8(uint8_t num) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

void printBinary16(uint16_t num) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

void assert8(uint8_t testValue, uint8_t truthValue)
{
    assert(testValue == truthValue);
}

void assert16(uint16_t testValue, uint16_t truthValue)
{
    assert(testValue == truthValue);
}