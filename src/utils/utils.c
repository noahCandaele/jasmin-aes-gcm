#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <immintrin.h>

#include "utils.h"

void print_test_return_status(int return_code)
{
	if (return_code == CODE_SUCCESS)
	{
		printf(GREEN_TEXT "Test successful\n" RESET_COLOR);
	}
	else if (return_code == CODE_WARNING)
	{
		printf(YELLOW_TEXT "Test returned with warnings\n" RESET_COLOR);
	}
	else if (return_code == CODE_INFO)
	{
		printf(BLUE_TEXT "Test returned, please check output manually\n" RESET_COLOR);
	}
	else
	{
		printf(RED_TEXT "Test failed\n" RESET_COLOR);
	}
	printf("\n");
}

static inline void native_cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
	asm volatile("cpuid"
				 : "=a" (*eax),
				   "=b" (*ebx),
				   "=c" (*ecx),
				   "=d" (*edx)
				 : "0" (*eax), "2" (*ecx));
}

static int check_cpuflags(void) {
	uint32_t eax = 1, ebx, ecx, edx;
	int arggl = 0;

	native_cpuid(&eax, &ebx, &ecx, &edx);

	if (!(ecx & (1 << 25))) {
		(void) fprintf(stderr, "Your CPU does not seem to have the AES instruction set\n");
		arggl += 1;
		return CODE_WARNING;
	}

	if (!(ecx & (1 << 28))) {
		(void) fprintf(stderr, "Your CPU does not seem to have the AVX instruction set\n");
		arggl += 1;
		return CODE_WARNING;
	}

	if (arggl) {
		(void) fprintf(stderr, "There is a high probability that the program triggers "
			"an \"Illegal instruction\" exception\n");
		return CODE_WARNING;
	}

	return CODE_SUCCESS;
}

int check_cpu_compatibility(void) {
	return check_cpuflags();
}

void print_uint8_array_as_binary(uint8_t* arr, size_t size, bool with_spaces) {
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < 8; ++j) {
			printf("%d", (arr[i] >> (7 - j)) & 1);
		}

		if(with_spaces) {
			// Add a space after every byte
			printf(" ");
		}
	}
	printf("\n");
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
		printf("%c", (char)arr[i]);

		if(with_spaces) {
			printf(" "); // Add a space after every character, since each ascii character is 1 byte (= 2 hex chars)
			// Add a space after every 4 characters
			if ((i + 1) % 2 == 0 && i + 1 < size) {
				printf(" ");
			}
		}
	}
	printf("\n");
}

size_t nb_bytes_hex_string(char* hex_string) {
	size_t length = strlen(hex_string);

    return ceil(length / 2.0);
}

bool compare_uint8_arrays(const uint8_t* array1, const uint8_t* array2, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (array1[i] != array2[i]) {
            return false; // Arrays are not equal
        }
    }
    return true; // Arrays are equal
}

void convert_hex_string_to_uint8_array(char* hex_string, uint8_t* uint8_array, size_t uint8_array_size) {
	for (size_t i = 0; i < uint8_array_size; ++i) {
		char hex[2] = { hex_string[i * 2], hex_string[i * 2 + 1] };
		// cast to int8_t
		uint8_array[i] = (uint8_t)strtol(hex, NULL, BASE_16);
	}
}

void convert_uint32_to_uint8_array(uint32_t value, uint8_t* arr) {
    for (size_t i = 0; i < NB_BYTES_32_BITS; ++i) {
        arr[i] = (value >> ((NB_BYTES_32_BITS - 1 - i) * 8)) & 0xFF;
    }
}

void convert_uint64_to_uint8_array(uint64_t value, uint8_t* arr) {
    for (size_t i = 0; i < NB_BYTES_64_BITS; ++i) {
        arr[i] = (value >> ((NB_BYTES_64_BITS - 1 - i) * 8)) & 0xFF;
    }
}

__m128i arr_to_u128(int8_t* arr) {
	return _mm_loadu_si128((__m128i *) arr);
}

void u128_to_arr(__m128i value, int8_t* arr) {
	_mm_storeu_si128((__m128i *)arr, value);
}

// __m256i arr_to_u256(int8_t* arr) {
//     __m128i* ptr = (__m128i*)arr;
//     __m256i result = _mm256_set_m128i(_mm_loadu_si128(ptr + 1), _mm_loadu_si128(ptr));
//     return result;
// }
