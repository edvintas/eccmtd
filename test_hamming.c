#include "hamming.h"
#include <stdio.h>
#include <string.h>

#define LH(x)	printf("0x%X\n", x)


int main() {
	printf("Testing hamming encode/decode\n");
	//char *input = "abc";
	char *input =  "abcde";
	LH(input[0]);
	block output[128];
	printf("-------------------\n");
	LH(output[0]);
	LH(output[1]);
	LH(output[2]);
	LH(output[3]);
	printf("-------------------\n");
	int retlen;
	encode(output, input, strlen(input) * 8, &retlen);
	LH(output[0]);
	LH(output[1]);
	printf("-------------------\n");
	printf("enc rlen = %d\n", retlen);
	char outputString[128];
	decode(outputString, output, retlen, &retlen);
	printf("dec rlen = %d, os = '%s'\n", retlen, outputString);
	printf("out string = %s\n", outputString);
	printf("Test completed\n");

	return 0;
}
