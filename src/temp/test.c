#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// void add1(int* strin);
void add1(char* strin);
void xor(char* strin);
uint64_t shift();

int main() {
	// input string
	char strin[] = "ab";
	add1(strin);
	printf("%s\n", strin);

	// int a = 300;
	// add1(&a);
	// printf("%d\n", a);

	// char* strin = (char*) malloc(sizeof(char) * 3);
	// strin[0] = ' ';
	// strin[1] = 'a';
	// strin[2] = 'b';
	// add1(strin);
	// printf("%s\n", strin);
	// strin[0] = ' ';
	// strin[1] = 'a';
	// strin[2] = 'b';
	// xor(strin);
	// printf("%s\n", strin);
	uint64_t a = 1;
	a = shift();
	printf("%lu\n", a);

	return 0;
}
