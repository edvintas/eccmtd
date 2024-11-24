#include "hamming.h"

#ifdef __KERNEL__
#include <linux/log2.h>
#else
#define pr_info printf
#define pr_err printf
#define pr_warn printf
#define ilog2 log2

#include <math.h>
#endif

// Function prototypes
bit getBit(block b, int i);                              // Function used to get a specific bit of a block
bit getCharBit(char b, int i);                           // Function used to get a specific bit of a char
block toggleBit(block b, int i);                         // Function used to toggle a specific bit of a block
block modifyBit(block n, int p, bit b);                  // Function used to modify a bit to a specific value
char modifyCharBit(char n, int p, bit b);                // Function used to modify a bit of a char to a specific value
int multipleXor(int *indicies, int len);                 // Function used to XOR all the elements of a list together (used to locate error and determine values of parity bits)

int encode(block output[], char *input, int len) {

	// Amount of bits in a block //
	int bits = sizeof(block) * 8;

	// Amount of bits per block used to carry the message //
	int messageBits = bits - ilog2(bits) - 1;

	// Amount of blocks needed to encode message //
	int blocks = (len * sizeof(input[0]) * 8) / messageBits;

	pr_info("Encoding: len = %d, bits = %d, mb = %d, blocks = %d", len, bits, messageBits, blocks);

	// Array of encoded blocks //
	block encoded[blocks+1];

	// Loop through each block //
	for(int i = 0; i < blocks+1; i++) {

		// Final encoded block variable //
		block thisBlock = 0;

		// Amount of "skipped" bits (used for parity) //
		int skipped = 0;

		// Count of how many bits are "on" //
		int onCount = 0;

		// Array of "on" bits //
		int onList[bits];

		// Loop through each message bit in this block to populate final block //
		for(int j = 0; j < bits; j++) {

			// Skip bit if reserved for parity bit //
			if((j & (j - 1)) == 0) {  // Check if j is a power of two or 0
				skipped++;
				continue;
			}

			bit thisBit;

			if(i != blocks) {

				// Current overall bit number //
				int currentBit = i*messageBits + (j-skipped);

				// Current character //
				int currentChar = currentBit/(sizeof(char)*8); // int division

				// Value of current bit //
				thisBit = currentBit < len*sizeof(char)*8 ? getCharBit(input[currentChar], currentBit-currentChar*8) : 0;
			}

			else {
				thisBit = getBit(len/8, j-skipped+(sizeof(block)*8-messageBits));
			}

			// If bit is "on", add to onList and onCount //
			if(thisBit) {
				onList[onCount] = j;
				onCount++;
			}

			// Populate final message block //
			thisBlock = modifyBit(thisBlock, j, thisBit);
		}

		// Calculate values of parity bits //
		block parityBits = multipleXor(onList, onCount);

		// Loop through skipped bits (parity bits) //
		for(int k = 1; k < skipped; k++) {  // skip bit 0

			// If bit is "on", add to onCount
			if(getBit(parityBits, sizeof(block)*8-skipped+k)) {
				onCount++;
			}

			// Add parity bit to final block //
			thisBlock = modifyBit(thisBlock, (skipped-k-1) << 1, getBit(parityBits, sizeof(block)*8-skipped+k));
		}

		// Add overall parity bit (total parity of onCount) //
		thisBlock = modifyBit(thisBlock, 0, onCount & 1);

		encoded[i] = thisBlock;
	}
	pr_info("Copying: %d / %d", len, ((blocks + 1) * sizeof(block)));
	memcpy(output, encoded, (blocks + 1) * sizeof(block));
	return 0;
}

int decode(char *outputBuff, block input[], int len) {
	pr_info("Decoding: %d", len);

	// Amount of bits in a block //
	int bits = sizeof(block) * 8;

	for(int b = 0; b < (len/sizeof(block)); b++) {
		// Count of how many bits are "on" //
		int onCount = 0;

		// Array of "on" bits //
		int onList[bits];

		// Populate onCount and onList //
		for(int i = 1; i < bits; i++) {
			getBit(input[b], i);
			if(getBit(input[b], i)) {
				onList[onCount] = i;
				onCount++;
			}
		}

		// Check for single errors //
		int errorLoc = multipleXor(onList, onCount);

		if(errorLoc) {

			// Check for multiple errors //
			if(!(onCount & 1 ^ getBit(input[b], 0))) {  // last bit of onCount (total parity) XOR first bit of block (parity bit)
				pr_err("\nMore than one error detected. Aborting.\n");
				return -1;
			}

			// Flip error bit //
			else {
				pr_warn("\nDetected error at position %d, flipping bit.\n", errorLoc);
				input[b] = toggleBit(input[b], (bits-1) - errorLoc);
			}
		}
	}

	// Initialise output string //
	char output[len];

	// Amount of bits per block used to carry the message //
	int messageBits = bits - ilog2(bits) - 1;

	int currentBit, currentChar;

	int chars = 0;

	for(int i = 0; i < len/sizeof(block); i++) {

		// Initialise variable to store amount of parity bits passed //
		int skipped = 0;

		// Loop through each message bit in this block to populate final block //
		for(int j = 0; j < bits; j++) {

			// Skip bit if reserved for parity bit //
			if((j & (j - 1)) == 0) {  // Check if j is a power of two or 0
				skipped++;
				continue;
			}

			// Current overall bit number //
			currentBit = i*messageBits + (j-skipped);

			// Current character //
			currentChar = currentBit/(sizeof(char)*8); // int division

			if(currentBit-currentChar*8 == 0 && currentBit != 0) {
				//putchar(' ');
			}

			// Value of current bit //
			bit thisBit = getBit(input[i], j);

			if(i != len/sizeof(block)-1) {

				// Populate final decoded character //
				output[currentChar] = modifyCharBit(output[currentChar], currentBit-currentChar*sizeof(char)*8, thisBit);
			}

			else {
				chars = modifyBit(chars, j-skipped+(sizeof(block)*8-messageBits), thisBit);
			}

		}
	}
	memcpy(outputBuff, output, len);
	return 0;
}

int multipleXor(int *indicies, int len) {
	int val = indicies[0];
	for(int i = 1; i < len; i++) {
		val = val ^ indicies[i];
	}
	return val;
}

block modifyBit(block n, int p, bit b) {
	return ((n & ~(1 << (sizeof(block)*8-1-p))) | (b << (sizeof(block)*8-1-p)));
}

char modifyCharBit(char n, int p, bit b) {
	return ((n & ~(1 << (sizeof(char)*8-1-p))) | (b << (sizeof(char)*8-1-p)));
}

bit getBit(block b, int i) {
	return (b << i) & ((sizeof(block)*8-1) << 1);
}

bit getCharBit(char b, int i) {
	return (b << i) & ((sizeof(char)*8-1) << 1);
}

block toggleBit(block b, int i) {
	return b ^ (1 << i);
}
