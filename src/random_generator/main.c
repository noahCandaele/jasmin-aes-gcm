#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <smmintrin.h>
#include <stdbool.h>

#include "jasmin_syscall.h"

#define BASE16 16

#define NB_BYTES_64_BITS 8

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

void convert_string_hex_to_uint8_array(char* hex_string, uint8_t* int8_array, size_t array_size) {
	for (size_t i = 0; i < array_size; ++i) {
		char hex[2] = { hex_string[i * 2], hex_string[i * 2 + 1] };
		// cast to int8_t
		int8_array[i] = (uint8_t)strtol(hex, NULL, BASE16);
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


int main()
{
	// TODO importer randombytes jasmin correctement (voir reponse Benjamin zulip)

	uint8_t plain[16];
	convert_string_hex_to_uint8_array("54776F204F6E65204E696E652054776F", plain, 16);

    print_uint8_array_as_hex(plain, 16, true);
	print_uint8_array_as_ascii(plain, 16, true);


	// uint64_t result = random64();
	// printf("Random 64-bit number: %" PRIu64 "\n", result);
	// char hex_string[17]; // 16-character hex string plus null-terminator
	// convert_uint64_to_uint8_array(result, hex_string);
	// print_uint8_array_as_hex(hex_string, 16, true);


	return 0;
}
