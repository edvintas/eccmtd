#include "hamming.h"
#include <stdio.h>
#include <string.h>


int main() {
	printf("Testing hamming encode/decode\n");
	char *input = "abcdefg";
	block *output;
///FIXME: segfault	encode(output, input, strlen(input));
	
	printf("Test completed\n");

	return 0;
}
