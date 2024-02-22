#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/utils.h"

// extern void aes_jazz2(uint8_t* key, uint8_t* plain, uint8_t* out_cipher);
extern void aes_jazz2(uint8_t* out_cipher, uint8_t* key, uint8_t* plain);
// invaes_jazz2(uint8_t* key, uint8_t* cipher, uint8_t* out_plain);

// int test_aes_generic(uint8_t* plain, uint8_t* key, uint8_t* cipher_expected) {
// 	// uint8_t cipher[NB_BYTES_128_BITS];
// 	uint8_t* cipher = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
// 	// aes_jazz2(key, plain, cipher);
// 	aes_jazz2(cipher, key, plain);
	
// 	printf("Expected cipher (hex)  : "); print_uint8_array_as_hex(cipher_expected, NB_BYTES_128_BITS, true);
// 	printf("Actual cipher   (hex)  : "); print_uint8_array_as_hex(cipher, NB_BYTES_128_BITS, true);
	
// 	if(!compare_uint8_arrays(cipher, cipher_expected, NB_BYTES_128_BITS)) {
// 		return CODE_FAIL;
// 	}

// 	return CODE_SUCCESS;
// }

// int test_invaes_generic(uint8_t* cipher, uint8_t* key, uint8_t* plain_expected) {
// 	uint8_t plain[NB_BYTES_128_BITS];
// 	invaes_jazz2(key, cipher, plain);
	
// 	printf("Expected plain (hex)   : "); print_uint8_array_as_hex(plain_expected, NB_BYTES_128_BITS, true);
// 	printf("Actual plain   (hex)   : "); print_uint8_array_as_hex(plain, NB_BYTES_128_BITS, true);
// 	printf("Expected plain (ascii) : "); print_uint8_array_as_ascii(plain_expected, NB_BYTES_128_BITS, true);
// 	printf("Actual plain   (ascii) : "); print_uint8_array_as_ascii(plain, NB_BYTES_128_BITS, true);
	
// 	if(!compare_uint8_arrays(plain, plain_expected, NB_BYTES_128_BITS)) {
// 		return CODE_FAIL;
// 	}

// 	return CODE_SUCCESS;
// }

// int test_yoda() {
// 	printf("######## Test yoda ########\n");

// 	// key
// 	// ASCII Thats my Kung Fu
// 	// hex 54 68 61 74 73 20 6D 79 20 4B 75 6E 67 20 46 75
	
// 	// plain
// 	// ASCII Two One Nine Two
// 	// hex 54 77 6F 20 4F 6E 65 20 4E 69 6E 65 20 54 77 6F

// 	// cipher
// 	// hex 29 C3 50 5F 57 14 20 F6 40 22 99 B3 1A 02 D7 3A
// 	// uint8_t key[NB_BYTES_128_BITS];
// 	// uint8_t plain[NB_BYTES_128_BITS];
// 	// uint8_t cipher_expected[NB_BYTES_128_BITS];
// 	// convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
// 	// convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
// 	// convert_hex_string_to_uint8_array("29C3505F571420F6402299B31A02D73A", cipher_expected, NB_BYTES_128_BITS);
// 	uint8_t key[] = {0x75, 0x46, 0x20, 0x67, 0x6E, 0x75, 0x4B, 0x20, 0x79, 0x6D, 0x20, 0x73, 0x74, 0x61, 0x68, 0x54};
// 	uint8_t plain[] = {0x6F, 0x77, 0x54, 0x20, 0x65, 0x6E, 0x69, 0x4E, 0x65, 0x20, 0x6F, 0x4E, 0x20, 0x77, 0x54};
// 	uint8_t cipher_expected[] = {0x3A, 0xD7, 0x02, 0x1A, 0xB3, 0x99, 0x22, 0x40, 0xF6, 0x20, 0x14, 0x57, 0x5F, 0x50, 0xC3, 0x29};
	
// 	// uint8_t key[] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
// 	// uint8_t plain[] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
// 	// uint8_t cipher_expected[] = {0x29, 0xC3, 0x50, 0x5F, 0x57, 0x14, 0x20, 0xF6, 0x40, 0x22, 0x99, 0xB3, 0x1A, 0x02, 0xD7, 0x3A};
	
// 	printf("AES\n");
// 	int res1 = test_aes_generic(plain, key, cipher_expected);
// 	// printf("INVAES\n");
// 	// int res2 = test_invaes_generic(cipher_expected, key, plain);

// 	// if (res1 > 0 || res2 > 0) {
// 	// 	return CODE_FAIL;
// 	// }

// 	return res1;
// }

// void print_raw_uint8_array(uint8_t* arr, int size) {
// 	for(int i = 0; i < size; i++) {
// 		printf("%02x ", (unsigned char)arr[i]);
// 	}
// 	printf("\n");
// }

// int test_yoda2() {
// 	printf("######## Test yoda 2 ########\n");

// 	// key
// 	// ASCII Thats my Kung Fu
// 	// hex 54 68 61 74 73 20 6D 79 20 4B 75 6E 67 20 46 75
	
// 	// plain
// 	// ASCII Two One Nine Two
// 	// hex 54 77 6F 20 4F 6E 65 20 4E 69 6E 65 20 54 77 6F

// 	// cipher
// 	// hex 29 C3 50 5F 57 14 20 F6 40 22 99 B3 1A 02 D7 3A
// 	char* test = malloc(16 * sizeof(char));
// 	uint8_t key[NB_BYTES_128_BITS];
// 	convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
// 	uint8_t plain[NB_BYTES_128_BITS];
// 	convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
// 	// aes_jazz2(key, plain, test);
// 	// aes_jazz2(test);
// 	// print test
// 	printf("returned %s\n", test);
// 	// uint8_t* key = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
// 	// uint8_t* plain = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
// 	// uint8_t* cipher = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
// 	// if (key == NULL || plain == NULL || cipher == NULL) {
// 	// 	printf("malloc failed");
// 	// 	return CODE_FAIL;
// 	// }

// 	// convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
// 	// convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
// 	// convert_hex_string_to_uint8_array("29C3505F571420F6402299B31A02D73A", cipher, NB_BYTES_128_BITS);
// 	// key = {0x75, 0x46, 0x20, 0x67, 0x6E, 0x75, 0x4B, 0x20, 0x79, 0x6D, 0x20, 0x73, 0x74, 0x61, 0x68, 0x54};
// 	// plain = {0x6F, 0x77, 0x54, 0x20, 0x65, 0x6E, 0x69, 0x4E, 0x65, 0x20, 0x6F, 0x4E, 0x20, 0x77, 0x54};
// 	// cipher_expected = {0x3A, 0xD7, 0x02, 0x1A, 0xB3, 0x99, 0x22, 0x40, 0xF6, 0x20, 0x14, 0x57, 0x5F, 0x50, 0xC3, 0x29};

// 	// uint8_t key = {0x75, 0x46, 0x20, 0x67, 0x6E, 0x75, 0x4B, 0x20, 0x79, 0x6D, 0x20, 0x73, 0x74, 0x61, 0x68, 0x54};
// 	// uint8_t plain[] = {0x6F, 0x77, 0x54, 0x20, 0x65, 0x6E, 0x69, 0x4E, 0x65, 0x20, 0x6F, 0x4E, 0x20, 0x77, 0x54};
// 	// uint8_t cipher_expected[] = {0x3A, 0xD7, 0x02, 0x1A, 0xB3, 0x99, 0x22, 0x40, 0xF6, 0x20, 0x14, 0x57, 0x5F, 0x50, 0xC3, 0x29};

// 	// uint8_t key[] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
// 	// uint8_t plain[] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
// 	// uint8_t cipher_expected[] = {0x29, 0xC3, 0x50, 0x5F, 0x57, 0x14, 0x20, 0xF6, 0x40, 0x22, 0x99, 0xB3, 0x1A, 0x02, 0xD7, 0x3A};
	
// 	// printf("AES\n");
// 	// uint8_t* cipher_computed = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
// 	// aes_jazz(key, plain, cipher_computed);
	
// 	// printf("Expected cipher (hex)  : "); print_raw_uint8_array(cipher, NB_BYTES_128_BITS);
// 	// printf("Actual cipher   (hex)  : "); print_raw_uint8_array(cipher_computed, NB_BYTES_128_BITS);

// 	// printf("INVAES\n");
// 	// uint8_t* plain_computed = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
// 	// invaes_jazz(key, cipher, plain_computed);
	
// 	// printf("Expected plain (hex)   : "); print_raw_uint8_array(plain, NB_BYTES_128_BITS);
// 	// printf("Actual plain   (hex)   : "); print_raw_uint8_array(plain_computed, NB_BYTES_128_BITS);

// 	return CODE_INFO;
// }

int test_yoda3() {
	// key
	// ASCII Thats my Kung Fu
	// hex 54 68 61 74 73 20 6D 79 20 4B 75 6E 67 20 46 75
	
	// plain
	// ASCII Two One Nine Two
	// hex 54 77 6F 20 4F 6E 65 20 4E 69 6E 65 20 54 77 6F

	// cipher
	// hex 29 C3 50 5F 57 14 20 F6 40 22 99 B3 1A 02 D7 3A

	// uint8_t key[] = {0x75, 0x46, 0x20, 0x67, 0x6E, 0x75, 0x4B, 0x20, 0x79, 0x6D, 0x20, 0x73, 0x74, 0x61, 0x68, 0x54};
	// uint8_t plain[] = {0x6F, 0x77, 0x54, 0x20, 0x65, 0x6E, 0x69, 0x4E, 0x65, 0x20, 0x6F, 0x4E, 0x20, 0x77, 0x54};
	// uint8_t cipher_expected[] = {0x3A, 0xD7, 0x02, 0x1A, 0xB3, 0x99, 0x22, 0x40, 0xF6, 0x20, 0x14, 0x57, 0x5F, 0x50, 0xC3, 0x29};

	// uint8_t key[] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
	// uint8_t plain[] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
	// uint8_t cipher_expected[] = {0x29, 0xC3, 0x50, 0x5F, 0x57, 0x14, 0x20, 0xF6, 0x40, 0x22, 0x99, 0xB3, 0x1A, 0x02, 0xD7, 0x3A};


	uint8_t* key = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* plain = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* cipher_expected = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (key == NULL || plain == NULL || cipher_expected == NULL) {
		printf("malloc failed\n");
		return CODE_FAIL;
	}

	key[0] = 0x54; key[1] = 0x68; key[2] = 0x61; key[3] = 0x74; key[4] = 0x73; key[5] = 0x20; key[6] = 0x6D; key[7] = 0x79; key[8] = 0x20; key[9] = 0x4B; key[10] = 0x75; key[11] = 0x6E; key[12] = 0x67; key[13] = 0x20; key[14] = 0x46; key[15] = 0x75;
	plain[0] = 0x54; plain[1] = 0x77; plain[2] = 0x6F; plain[3] = 0x20; plain[4] = 0x4F; plain[5] = 0x6E; plain[6] = 0x65; plain[7] = 0x20; plain[8] = 0x4E; plain[9] = 0x69; plain[10] = 0x6E; plain[11] = 0x65; plain[12] = 0x20; plain[13] = 0x54; plain[14] = 0x77; plain[15] = 0x6F;
	cipher_expected[0] = 0x29; cipher_expected[1] = 0xC3; cipher_expected[2] = 0x50; cipher_expected[3] = 0x5F; cipher_expected[4] = 0x57; cipher_expected[5] = 0x14; cipher_expected[6] = 0x20; cipher_expected[7] = 0xF6; cipher_expected[8] = 0x40; cipher_expected[9] = 0x22; cipher_expected[10] = 0x99; cipher_expected[11] = 0xB3; cipher_expected[12] = 0x1A; cipher_expected[13] = 0x02; cipher_expected[14] = 0xD7; cipher_expected[15] = 0x3A;

	// key[0] = 0x75; key[1] = 0x46; key[2] = 0x20; key[3] = 0x67; key[4] = 0x6E; key[5] = 0x75; key[6] = 0x4B; key[7] = 0x20; key[8] = 0x79; key[9] = 0x6D; key[10] = 0x20; key[11] = 0x73; key[12] = 0x74; key[13] = 0x61; key[14] = 0x68; key[15] = 0x54;
	// plain[0] = 0x6F; plain[1] = 0x77; plain[2] = 0x54; plain[3] = 0x20; plain[4] = 0x65; plain[5] = 0x6E; plain[6] = 0x69; plain[7] = 0x4E; plain[8] = 0x65; plain[9] = 0x20; plain[10] = 0x6F; plain[11] = 0x4E; plain[12] = 0x20; plain[13] = 0x77; plain[14] = 0x54; plain[15] = 0x6F;
	// cipher_expected[0] = 0x3A; cipher_expected[1] = 0xD7; cipher_expected[2] = 0x02; cipher_expected[3] = 0x1A; cipher_expected[4] = 0xB3; cipher_expected[5] = 0x99; cipher_expected[6] = 0x22; cipher_expected[7] = 0x40; cipher_expected[8] = 0xF6; cipher_expected[9] = 0x20; cipher_expected[10] = 0x14; cipher_expected[11] = 0x57; cipher_expected[12] = 0x5F; cipher_expected[13] = 0x50; cipher_expected[14] = 0xC3; cipher_expected[15] = 0x29;

	uint8_t* cipher = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	aes_jazz2(cipher, key, plain);
	
	printf("Expected cipher (hex)  : "); print_uint8_array_as_hex(cipher_expected, NB_BYTES_128_BITS, true);
	printf("Actual cipher   (hex)  : "); print_uint8_array_as_hex(cipher, NB_BYTES_128_BITS, true);

	return CODE_INFO;
}

int test_yoda4() {
	uint8_t key[NB_BYTES_128_BITS];
	uint8_t plain[NB_BYTES_128_BITS];
	uint8_t cipher_expected[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("29C3505F571420F6402299B31A02D73A", cipher_expected, NB_BYTES_128_BITS);
	// uint8_t key[] = {0x75, 0x46, 0x20, 0x67, 0x6E, 0x75, 0x4B, 0x20, 0x79, 0x6D, 0x20, 0x73, 0x74, 0x61, 0x68, 0x54};
	// uint8_t plain[] = {0x6F, 0x77, 0x54, 0x20, 0x65, 0x6E, 0x69, 0x4E, 0x65, 0x20, 0x6F, 0x4E, 0x20, 0x77, 0x54};
	// uint8_t cipher_expected[] = {0x3A, 0xD7, 0x02, 0x1A, 0xB3, 0x99, 0x22, 0x40, 0xF6, 0x20, 0x14, 0x57, 0x5F, 0x50, 0xC3, 0x29};
	
	// uint8_t key[] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
	// uint8_t plain[] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
	// uint8_t cipher_expected[] = {0x29, 0xC3, 0x50, 0x5F, 0x57, 0x14, 0x20, 0xF6, 0x40, 0x22, 0x99, 0xB3, 0x1A, 0x02, 0xD7, 0x3A};

	uint8_t cipher[NB_BYTES_128_BITS];
	// aes_jazz2(key, plain, cipher);
	aes_jazz2(cipher, key, plain);
	
	printf("Expected cipher (hex)  : "); print_uint8_array_as_hex(cipher_expected, NB_BYTES_128_BITS, true);
	printf("Actual cipher   (hex)  : "); print_uint8_array_as_hex(cipher, NB_BYTES_128_BITS, true);
	
	if(!compare_uint8_arrays(cipher, cipher_expected, NB_BYTES_128_BITS)) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int main()
{
	// print_test_return_status(test_yoda());
	// print_test_return_status(test_yoda2());
	print_test_return_status(test_yoda3());
	print_test_return_status(test_yoda4());

	return CODE_SUCCESS;
}
