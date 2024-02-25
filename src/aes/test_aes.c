#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/utils.h"

extern void aes_jazz(uint8_t* key, uint8_t* plain, uint8_t* out_cipher);
extern void invaes_jazz(uint8_t* key, uint8_t* cipher, uint8_t* out_plain);

int test_aes_generic(uint8_t* plain, uint8_t* key, uint8_t* cipher_expected) {
	uint8_t cipher[NB_BYTES_128_BITS];
	aes_jazz(key, plain, cipher);
	
	printf("Expected cipher (hex):   "); print_uint8_array_as_hex_in_order(cipher_expected, NB_BYTES_128_BITS, true);
	printf("Actual cipher   (hex):   "); print_uint8_array_as_hex_in_order(cipher, NB_BYTES_128_BITS, true);
	
	if(!compare_uint8_arrays(cipher, cipher_expected, NB_BYTES_128_BITS)) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_invaes_generic(uint8_t* cipher, uint8_t* key, uint8_t* plain_expected) {
	uint8_t plain[NB_BYTES_128_BITS];
	invaes_jazz(key, cipher, plain);
	
	printf("Expected plain (hex):    "); print_uint8_array_as_hex_in_order(plain_expected, NB_BYTES_128_BITS, true);
	printf("Actual plain   (hex):    "); print_uint8_array_as_hex_in_order(plain, NB_BYTES_128_BITS, true);
	printf("Expected plain (ascii):  "); print_uint8_array_as_ascii_in_order(plain_expected, NB_BYTES_128_BITS, true);
	printf("Actual plain   (ascii):  "); print_uint8_array_as_ascii_in_order(plain, NB_BYTES_128_BITS, true);
	
	if(!compare_uint8_arrays(plain, plain_expected, NB_BYTES_128_BITS)) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_yoda() {
	printf("######## Test yoda ########\n");

	// key
	// ASCII Thats my Kung Fu
	// hex 54 68 61 74 73 20 6D 79 20 4B 75 6E 67 20 46 75
	
	// plain
	// ASCII Two One Nine Two
	// hex 54 77 6F 20 4F 6E 65 20 4E 69 6E 65 20 54 77 6F

	// cipher
	// hex 29 C3 50 5F 57 14 20 F6 40 22 99 B3 1A 02 D7 3A

	// convert_hex_string_to_uint8_array_in_order("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array_in_order("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array_in_order("29C3505F571420F6402299B31A02D73A", cipher_expected, NB_BYTES_128_BITS);

	// uint8_t key[] = {0x75, 0x46, 0x20, 0x67, 0x6E, 0x75, 0x4B, 0x20, 0x79, 0x6D, 0x20, 0x73, 0x74, 0x61, 0x68, 0x54};
	// uint8_t plain[] = {0x6F, 0x77, 0x54, 0x20, 0x65, 0x6E, 0x69, 0x4E, 0x65, 0x20, 0x6F, 0x4E, 0x20, 0x77, 0x54};
	// uint8_t cipher_expected[] = {0x3A, 0xD7, 0x02, 0x1A, 0xB3, 0x99, 0x22, 0x40, 0xF6, 0x20, 0x14, 0x57, 0x5F, 0x50, 0xC3, 0x29};
	
	// uint8_t key[] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
	// uint8_t plain[] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
	// uint8_t cipher_expected[] = {0x29, 0xC3, 0x50, 0x5F, 0x57, 0x14, 0x20, 0xF6, 0x40, 0x22, 0x99, 0xB3, 0x1A, 0x02, 0xD7, 0x3A};

	uint8_t key[NB_BYTES_128_BITS];
	uint8_t plain[NB_BYTES_128_BITS];
	uint8_t cipher_expected[NB_BYTES_128_BITS];
	convert_ascii_string_to_uint8_array_in_order("Thats my Kung Fu", key, NB_BYTES_128_BITS);
	convert_ascii_string_to_uint8_array_in_order("Two One Nine Two", plain, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order("29C3505F571420F6402299B31A02D73A", cipher_expected, NB_BYTES_128_BITS);

	printf("AES\n");
	int code1 = test_aes_generic(plain, key, cipher_expected);

	printf("\nINVAES\n");
	int code2 = test_invaes_generic(cipher_expected, key, plain);
	
	if (code1 != CODE_SUCCESS || code2 != CODE_SUCCESS) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int main()
{
	print_test_return_status(test_yoda());

	return CODE_SUCCESS;
}
