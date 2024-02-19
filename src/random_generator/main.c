#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <smmintrin.h>
#include <stdbool.h>

#include "../utils/utils.h"
// TODO importer randombytes jasmin correctement (voir reponse Benjamin zulip)
#include "../../lib/jasmin_syscall/jasmin_syscall.h"

extern uint64_t random32();
extern uint64_t random64();
extern __m128i random128();
extern __m128i iv_init();
extern uint64_t endianness64();
extern __m128i endianness128();

int test_random32() {
	printf("######## Test random 32 bits ########\n");
	uint32_t res = random64();

	uint8_t arr[NB_BYTES_32_BITS];
	convert_uint32_to_uint8_array(res, arr);
	printf("bin: "); print_uint8_array_as_binary(arr, NB_BYTES_32_BITS, true);
	printf("hex: "); print_uint8_array_as_hex(arr, NB_BYTES_32_BITS, true);

	return CODE_INFO;
}

int test_random64() {
	printf("######## Test random 64 bits ########\n");
	uint64_t res = random64();

	uint8_t arr[NB_BYTES_64_BITS];
	convert_uint64_to_uint8_array(res, arr);
	printf("bin: "); print_uint8_array_as_binary(arr, NB_BYTES_64_BITS, true);
	printf("hex: "); print_uint8_array_as_hex(arr, NB_BYTES_64_BITS, true);

	return CODE_INFO;
}

int test_random128() {
	printf("######## Test random 128 bits ########\n");

	__m128i res = random128();
	uint8_t arr[NB_BYTES_128_BITS];
	u128_to_arr(res, arr);

	printf("bin: "); print_uint8_array_as_binary(arr, NB_BYTES_128_BITS, true);
	printf("hex: "); print_uint8_array_as_hex(arr, NB_BYTES_128_BITS, true);

	return CODE_INFO;
}

int test_iv_init() {
	printf("######## Test IV init ########\n");

	__m128i res = iv_init();
	uint8_t arr[NB_BYTES_128_BITS];
	u128_to_arr(res, arr);

	printf("bin: "); print_uint8_array_as_binary(arr, NB_BYTES_128_BITS, true);
	printf("hex: "); print_uint8_array_as_hex(arr, NB_BYTES_128_BITS, true);

	return CODE_INFO;
}

int test_endianness64() {
	printf("######## Test endianness 64 bit ########\n");

	uint64_t res = endianness64();

	uint8_t arr[NB_BYTES_64_BITS];
	convert_uint64_to_uint8_array(res, arr);
	printf("hex: "); print_uint8_array_as_hex(arr, NB_BYTES_64_BITS, true);

	return CODE_INFO;
}

int test_endianness128() {
	printf("######## Test endianness 128 bit ########\n");

	__m128i res = endianness128();
	uint8_t arr[NB_BYTES_128_BITS];
	u128_to_arr(res, arr);

	printf("hex: "); print_uint8_array_as_hex(arr, NB_BYTES_128_BITS, true);

	return CODE_INFO;
}

int main()
{
	print_test_return_status(test_random32());
	print_test_return_status(test_random64());
	print_test_return_status(test_random128());
	print_test_return_status(test_iv_init());
	print_test_return_status(test_endianness64());
	print_test_return_status(test_endianness128());

	return 0;
}
