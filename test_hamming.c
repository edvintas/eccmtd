#include "hamming.h"
#include <stdio.h>
#include <string.h>


int main() {
	printf("Testing hamming encode/decode\n");
	char *input = "abcdefg";
	block output[128];
	int retlen;
	encode(output, input, strlen(input), &retlen);
	printf("rlen = %d\n", retlen);
	printf("Test completed\n");

	return 0;
}
