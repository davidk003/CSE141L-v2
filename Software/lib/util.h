#include <stdint.h>

typedef struct{
    uint8_t test;
    uint8_t truth;
} test8;

typedef struct{
    uint16_t test;
    uint16_t truth;
    
} test16;

void printBinary8(uint8_t num);

void printBinary16(uint16_t num);

void assert8(uint8_t testValue, uint8_t truthValue);

void assert16(uint16_t testValue, uint16_t truthValue);
