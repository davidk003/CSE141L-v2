/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdint.h>

// The shiftLeft function as provided earlier
void shiftLeft(uint8_t s, uint8_t *A, uint8_t *B, uint8_t *C, uint8_t *D) {
    uint8_t t;
    t = s >> 3;        // t = numBytesShift (number of full byte shifts)
    s = s & 7;         // s = numBitsShift (remaining bits to shift within bytes)

    // Shift bytes according to numBytesShift
        if(t == 1)
        {   
            *A = *B;
            *B = *C;
            *C = *D;
            *D = 0;
            
        }
        if(t == 2)
        {
            *A = *C;
            *B = *D;
            *C = 0;
            *D = 0;
        }
        if(t == 3)
        {
            *A = *D;
            *B = 0;
            *C = 0;
            *D = 0;
        }
        if(t == 4)
        {
            *A = 0;
            *B = 0;
            *C = 0;
            *D = 0;
        }



    // If there are remaining bits to shift, perform bit-level shifting
    if (s != 0) {
        t = 8 - s;              // t = 8 - numBitsShift

        // Shift A
        t = *B >> t;
        *A = *A << s;
        *A = *A | t;

        // Shift B
        t = 8 - s;
        t = *C >> t;
        *B = *B << s;
        *B = *B | t;

        // Shift C
        t = 8 - s;
        t = *D >> t;
        *C = *C << s;
        *C = *C | t;

        // Shift D
        *D = *D << s;
    }
}

// Helper function to print a uint8_t variable in binary
void printBinary(uint8_t x) {
    for (int i = 7; i >= 0; i--) {
        printf("%c", (x & (1 << i)) ? '1' : '0');
    }
}

int main() {
    // Initial values for the variables (specified in binary)
    uint8_t A_ini = 0b10101010; // Initial value for A
    uint8_t B_ini = 0b01010101; // Initial value for B
    uint8_t C_ini = 0b11001100; // Initial value for C
    uint8_t D_ini = 0b00110011; // Initial value for D

    // Array of shift amounts to test
    uint8_t shiftAmounts[] = {0, 1, 7, 8, 9, 12, 15, 16, 24, 31, 32};
    int numTests = sizeof(shiftAmounts) / sizeof(shiftAmounts[0]);

    // Perform the tests
    for (int i = 0; i < numTests; i++) {
        uint8_t A = A_ini;
        uint8_t B = B_ini;
        uint8_t C = C_ini;
        uint8_t D = D_ini;

        uint8_t s = shiftAmounts[i];

        printf("\nTest %d: Shift amount = %d\n", i + 1, s);
        printf("Before shifting:\n");
        printf("A = "); printBinary(A);
        printf("_"); printBinary(B);
        printf("_"); printBinary(C);
        printf("_"); printBinary(D); printf("\n");

        shiftLeft(s, &A, &B, &C, &D);

        printf("After shifting left by %d bits:\n", s);
        printf("A = "); printBinary(A);
        printf("_"); printBinary(B);
        printf("_"); printBinary(C);
        printf("_"); printBinary(D); printf("\n");
    }

    return 0;
}