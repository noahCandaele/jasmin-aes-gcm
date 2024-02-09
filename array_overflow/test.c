#include <stdio.h>

// define de la size du tableau
#define SIZE 16

extern unsigned long long main_function();

int main() {
    // Initialize the list
    unsigned char list[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    
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

    return 0;
}
