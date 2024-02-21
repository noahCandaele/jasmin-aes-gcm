#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <immintrin.h>

#include "utils.h"

// Convention: least signigicant bytes are at the start of the array

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
	for (int i = size - 1; i >= 0; i--) {
		for (int j = 7; j >= 0; j--) {
			printf("%d", (arr[i] >> j) & 1);
		}
		if(with_spaces) printf(" "); // Add space after every byte
	}
	printf("\n");
}

void print_uint8_array_as_hex(uint8_t *arr, size_t size, bool with_spaces)
{
	for (int i = size - 1; i >= 0; i--)
	{
		printf("%02x", (unsigned char)arr[i]);

		if(with_spaces) {
			printf(" "); // Add a space after every byte
		}
	}
	printf("\n");
}

void print_uint8_array_as_ascii(uint8_t* arr, size_t size, bool with_spaces) {
	for (size_t i = 0; i < size; ++i) {
		printf("%c", (char)arr[size-i-1]);

		if(with_spaces) {
			printf("  "); // Add a space after every character, since each ascii character is 1 byte (= 2 hex chars), also add a space after every byte
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
		uint8_array[uint8_array_size-i-1] = (uint8_t)strtol(hex, NULL, BASE_16);
	}
}

void convert_ascii_string_to_uint8_array(char* ascii_string, uint8_t* uint8_array, size_t uint8_array_size) {
	for (size_t i = 0; i < uint8_array_size; ++i) {
		uint8_array[uint8_array_size-i-1] = (uint8_t)ascii_string[i];
	}
}
