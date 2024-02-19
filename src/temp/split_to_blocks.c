#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void shrinkstr(char* strin, char* strout, int len, int blockID);

int main() {
	// input string
	char strin[] = "Hello, World! This is a test. I hope it works.";
	// malloc strout
	char* strout = (char*)malloc(sizeof(char) * 16);
	// fet N id block
	long size = 1;
	int blockID = 0;	
	
	while (size>0){
		shrinkstr(strin, strout, strlen(strin), blockID);
		printf("%s\n", strout);
		size = strlen(strout);
		printf("strlen: %ld\n", size);
		blockID++;
	}

	return 0;
}
