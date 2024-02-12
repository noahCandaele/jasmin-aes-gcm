#include <stdio.h>

// define de la size du tableau
#define SIZE 16

extern unsigned long main_function(char* list);

int main() {
	// Initialize the list
	unsigned char list[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	size_t list_size = SIZE * SIZE * sizeof(char);
	unsigned char *list = (char *)malloc(list_size);

	if (my_sbox == NULL) {
		// Allocation failed, handle the error
		return 1;
	}

	for (int i = 0; i < SIZE; i++) {
		printf("%d ", list[i]);
	}
    
	printf("Initial list:\n");
	for (int i = 0; i < SIZE; i++) {
		printf("%d ", list[i]);
	}
	// Call the assembly function
	main_function(list);

	printf("\n");
	// Print the modified list
	printf("Modified list:\n");
	for (int i = 0; i < SIZE; i++) {
		printf("%d ", list[i]);
	}
	printf("\n");

	free(list);
	return 0;
}
