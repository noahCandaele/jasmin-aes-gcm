#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <smmintrin.h>
#include <stdbool.h>

// TODO importer randombytes jasmin correctement (voir reponse Benjamin zulip)
#include "../../lib/jasmin_syscall/jasmin_syscall.h"

#define BASE16 16

#define NB_BYTES_64_BITS 8
#define NB_BYTES_128_BITS 16

extern uint64_t random64();

// __m128i arr_to_u128(int8_t *arr)
// {
// 	return _mm_loadu_si128((__m128i *)arr);
// }

// void u128_to_arr(__m128i value, int8_t *arr)
// {
// 	_mm_storeu_si128((__m128i *)arr, value);
// }


// void convert_uint64_to_uint8_array(uint64_t value, uint8_t *arr)
// {
// 	memcpy(arr, &value, NB_BYTES_64_BITS);
// }

void convert_hex_string_to_uint8_array(char* hex_string, uint8_t* uint8_array, size_t array_size) {
	for (size_t i = 0; i < array_size; ++i) {
		char hex[2] = { hex_string[i * 2], hex_string[i * 2 + 1] };
		// cast to int8_t
		uint8_array[i] = (uint8_t)strtol(hex, NULL, BASE16);
	}
}

void convert_uint64_to_uint8_array(uint64_t value, uint8_t* arr) {
	for (size_t i = 0; i < NB_BYTES_64_BITS; ++i) {
		arr[i] = (value >> (i * 8)) & 0xFF;
	}
}

void print_uint8_array_as_hex(uint8_t *arr, size_t size, bool with_spaces)
{
	for (int i = 0; i < size; i++)
	{
		printf("%02x", (unsigned char)arr[i]);

		if(with_spaces) {
			// Add a space after every 4 characters
			if ((i + 1) % 2 == 0 && i + 1 < size) {
				printf(" ");
			}
		}
	}
	printf("\n");
}

void print_uint8_array_as_ascii(uint8_t* arr, size_t size, bool with_spaces) {
	for (size_t i = 0; i < size; ++i) {
		printf("%c ", (char)arr[i]);

		if(with_spaces) {
			// Add a space after every 4 characters
			if ((i + 1) % 2 == 0 && i + 1 < size) {
				printf(" ");
			}
		}
	}
	printf("\n");
}

void print_uint8_array_as_binary(uint8_t* arr, size_t size, bool with_spaces) {
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < 8; ++j) {
			printf("%d", (arr[i] >> (7 - j)) & 1);
		}

		if(with_spaces) {
			// Add a space after every 16 characters
			if ((i + 1) % 2 == 0 && i + 1 < size) {
				printf(" ");
			}
		}
	}
	printf("\n");
}

void test_print() {
	uint8_t plain[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);

    print_uint8_array_as_hex(plain, NB_BYTES_128_BITS, true);
	print_uint8_array_as_ascii(plain, NB_BYTES_128_BITS, true);
	print_uint8_array_as_binary(plain, NB_BYTES_128_BITS, true);
}


int main()
{
	// test_print();


	int8_t* arr = (int8_t*)malloc(NB_BYTES_64_BITS * sizeof(int8_t));
	if (arr == NULL) {
		printf("Malloc failed");
		return 1;
	}
	convert_uint64_to_uint8_array(1, arr);
	print_uint8_array_as_hex(arr, NB_BYTES_64_BITS, true);
	print_uint8_array_as_binary(arr, NB_BYTES_64_BITS, true);

	// int8_t* arr = (int8_t*)malloc(NB_BYTES_64_BITS * sizeof(int8_t));
	// if (arr == NULL) {
	// 	printf("Malloc failed");
	// 	return 1;
	// }


	// uint64_t result = random64();
	// convert_uint64_to_uint8_array(result, arr);
	// print_uint8_array_as_hex(arr, NB_BYTES_64_BITS, true);

	return 0;
}
