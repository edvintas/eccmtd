#ifndef __HAMMING_H__
#define __HAMMING_H__

#include <linux/module.h>

// Definitions
typedef unsigned int block;    // 32 bits
typedef unsigned char bit;

// Function prototypes
int decode(char *input, int len, char *output);          // Function used to decode Hamming code
int encode(char *input, int len, char *output);

#endif
