#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define SBOX_SIDE 16

// extern void init_aes_sbox(uint64_t* sbox);
extern void compute_array(uint8_t* sbox);
extern uint8_t sbox_get(uint64_t i);

int main() {
	size_t array_size = SBOX_SIDE * SBOX_SIDE * sizeof(uint8_t);
	uint8_t *my_sbox = (uint8_t *)malloc(array_size);

	if (my_sbox == NULL) {
		// Allocation failed, handle the error
		return 1;
	}

	// Initialize the array to 0
	for (int i = 0; i < SBOX_SIDE * SBOX_SIDE; ++i) {
		my_sbox[i] = 0;
	}

	// printf("Size: %ld\n", sizeof(my_sbox)/sizeof(uint64_t));
	printf("Address of the array: %p\n", my_sbox);
	// Call the Jasmin function to change the element at index 3 to 42
	compute_array(my_sbox);
	// uint8_t res = sbox_get(255);
	// printf("res: %" PRIu8 "\n",res);

	// Print the modified array
	for (int line = 0; line < SBOX_SIDE; line++) {
		// printf("current line %d\n", line);
		for (int col = 0; col<SBOX_SIDE; col++){
			printf("%" PRIx8 " ", my_sbox[col+SBOX_SIDE*line]);
		}
		printf("\n");
	}

	free(my_sbox);
	return 0;
}
