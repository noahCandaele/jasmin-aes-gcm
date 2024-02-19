#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <smmintrin.h>
#include <immintrin.h>
#include <sys/types.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define NB_BYTES_128_BITS 16

void aes_count(char* strin, char* strout, int len, __m128i key, __m128i iv);

__m128i arr_to_u128(int8_t* arr) {
	return _mm_loadu_si128((__m128i *) arr);
}

void string_hex_to_int8_array(char hex_string[], int8_t* int8_array, size_t array_size) {
	for (size_t i = 0; i < array_size; ++i) {
		char hex[2] = { hex_string[i * 2], hex_string[i * 2 + 1] };
		// cast to int8_t
		int8_array[i] = (int8_t)strtol(hex, NULL, 16);
	}
}

int main() {
	int8_t* key = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* IV = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));

	string_hex_to_int8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	string_hex_to_int8_array("f34481ec3cc627bacd5dc3fb08f273e6", IV, NB_BYTES_128_BITS);
	// print key 
	for (size_t i = 0; i < NB_BYTES_128_BITS; ++i) {
		printf("%02x", (unsigned char)IV[i]);
	}
	printf("\n");
	// string_hex_to_int8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	// string_hex_to_int8_array("479be376295fc8547276c4c9a76d2822", cipher, NB_BYTES_128_BITS);

	// string_hex_to_int8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// string_hex_to_int8_array("54776F204F6E65204E696E652054776F", IV, NB_BYTES_128_BITS);
	// string_hex_to_int8_array("00000000000000000000000000000000", IV, NB_BYTES_128_BITS);

	char strin_no[] = "Two One Nine Two";
	char* strin = (char*)malloc(sizeof(char) * strlen(strin_no));
	strcpy(strin, strin_no);
	// malloc strout
	char* strout = (char*)malloc(sizeof(char) * 16 * (strlen(strin)/16 + 1));
	// verify if malloc worked
	if (strout == NULL) {
		printf("Malloc failed\n");
		return 1;
	}
	// fet N id block
	long size = strlen(strin);
	printf("Input string size: %ld\n", size);
	aes_count(strin, strout, size, arr_to_u128(key), arr_to_u128(IV));
	printf("Output string: %s\n", strout);
	printf("Output string size: %ld\n", strlen(strout));
	// print strout as hex
	for (size_t i = 0; i < strlen(strout); ++i) {
		printf("%02x", (unsigned char)strout[i]);
	}
	printf("\n");

	return 0;
}
