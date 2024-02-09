#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SBOX_SIDE 16

extern void init_aes_sbox(uint64_t* sbox);

int main() {
    size_t array_size = SBOX_SIDE * SBOX_SIDE * sizeof(uint64_t);
    uint64_t *my_sbox = (uint64_t *)malloc(array_size);

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
    init_aes_sbox(my_sbox);

    // Print the modified array
    for (int line = 0; line < SBOX_SIDE; line++) {
        // printf("current line %d\n", line);
        for (int col = 0; col<SBOX_SIDE; col++){
            printf("%02lx ", my_sbox[col+SBOX_SIDE*line]);
        }
        printf("\n");
    }

    free(my_sbox);
    return 0;
}
