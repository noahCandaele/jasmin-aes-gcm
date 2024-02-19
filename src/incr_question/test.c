#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// void add1(int* strin);
void add1(char* strin);

int main() {
	// input string
	// char strin[] = "ab";
	// add1(strin);
	// printf("%s\n", strin);

	// int a = 300;
	// add1(&a);
	// printf("%d\n", a);

	// char* strin = (char*)malloc(sizeof(char) * 10);
	// // strin = "ab";
	// strin[0] = 'a';
	// strin[1] = 'b';
	uint8_t strin[] = {65, 66};
	printf("%s\n", strin);
	add1(strin);
	printf("%s\n", strin);

	return 0;
}
