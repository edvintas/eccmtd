#ifndef __HAMMING_H__
#define __HAMMING_H__

// Definitions
#define block unsigned short    // 16 bits
#define bit int                // 8 bits (only last is used)
#define size_t unsigned int
#define FILE char
#define false 0
#define true 1
#define bool int

// Function prototypes
void decode(block input[], int len, FILE *ptr);          // Function used to decode Hamming code
void encode(char *input, int len, FILE *ptr);

#endif
