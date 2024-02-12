#include <stdio.h>
#include <stdint.h>
// TODO à enlever
#include "../secret.h"

uint32_t hello_world();

int main() {
	uint32_t result = hello_world();
	printf("result: %u\n", result);

	return 0;
}
