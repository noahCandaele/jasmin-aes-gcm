#include <stdio.h>
#include <stdint.h>

uint32_t hello();

int main() {
   uint32_t result = hello();
   printf("result: %u\n", result);
   
   return 0;
}
