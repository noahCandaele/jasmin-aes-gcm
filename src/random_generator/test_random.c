#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <smmintrin.h>
#include <stdbool.h>

#include "../utils/utils.h"

extern void random32_jazz(uint8_t* out_rand);
extern void random64_jazz(uint8_t* out_rand);
extern void random128_jazz(uint8_t* out_rand);
extern void iv_init_jazz(uint8_t* out_iv);

int test_random32() {
	printf("######## Test random 32 bits ########\n");

	uint8_t rand[NB_BYTES_32_BITS];
	random32_jazz(rand);
	printf("rand (hex): "); print_uint8_array_as_hex(rand, NB_BYTES_32_BITS, true);

	return CODE_INFO;
}

int test_random64() {
	printf("######## Test random 64 bits ########\n");

	uint8_t rand[NB_BYTES_64_BITS];
	random64_jazz(rand);
	printf("rand (hex): "); print_uint8_array_as_hex(rand, NB_BYTES_64_BITS, true);

	return CODE_INFO;
}

int test_random128() {
	printf("######## Test random 128 bits ########\n");

	uint8_t rand[NB_BYTES_128_BITS];
	random128_jazz(rand);
	printf("rand (hex): "); print_uint8_array_as_hex(rand, NB_BYTES_128_BITS, true);

	return CODE_INFO;
}

int test_iv_init() {
	printf("######## Test IV init ########\n");

	uint8_t iv[NB_BYTES_128_BITS];
	iv_init_jazz(iv);
	printf("iv (hex): "); print_uint8_array_as_hex(iv, NB_BYTES_128_BITS, true);

	return CODE_INFO;
}

int main()
{
	print_test_return_status(test_random32());
	print_test_return_status(test_random64());
	print_test_return_status(test_random128());

	print_test_return_status(test_iv_init());

	return 0;
}
