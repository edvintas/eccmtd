#ifndef __HAMMING_H__
#define __HAMMING_H__

#ifdef __KERNEL__
#include <linux/module.h>
#else
#include <stdbool.h>
#endif

// Definitions
typedef unsigned short block;    // 32 bits / 20% data for ECC approx
typedef bool bit;

// Function prototypes
int decode(char *output, block input[], int len, int *retlen);          // Function used to decode Hamming code
int encode(block *output, char *input, int len, int *retlen);

#endif
