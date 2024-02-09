#include <stdint.h>
#include <stdio.h>

extern void init_aes_sbox(uint64_t myList);

int main() {
    const int side = 16;
    uint64_t my_sbox[side*side];

    // Initialize the array to 0
    for (int i = 0; i < side * side; ++i) {
        my_sbox[i] = 0;
    }

    printf("Size: %ld\n", sizeof(my_sbox)/sizeof(uint64_t));
    printf("Address of the array: %p\n", my_sbox);
    // Call the Jasmin function to change the element at index 3 to 42
    init_aes_sbox(*my_sbox);

    // Print the modified array
    for (int line = 0; line < side; line++) {
        // printf("current line %d\n", line);
        for (int col = 0; col<side; col++){
            printf("%02lx ", my_sbox[col+side*line]);
        }
        printf("\n");
    }

    return 0;
}
