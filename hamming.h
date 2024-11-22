#ifndef __HAMMING_H__
#define __HAMMING_H__

#include <linux/module.h>

// Definitions
typedef unsigned int block;    // 32 bits
typedef unsigned char bit;

// Function prototypes
int decode(char *output, char *input, int len);          // Function used to decode Hamming code
int encode(char *output, char *input, int len);

#endif
