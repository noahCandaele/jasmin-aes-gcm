#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "../utils/utils.h"

// void aes_count(int8_t* plain, int8_t* out_cipher, int length, int8_t* key);
void aes_count_iv(int8_t* plain, int8_t* out_cipher, int length, int8_t* key, int8_t* iv);

int test_yoda(){
	printf("######## Test yoda ########\n");

	uint8_t key[] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
	uint8_t plain[] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65, 0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
	uint8_t cipher_expected[] = {0x29, 0xC3, 0x50, 0x5F, 0x57, 0x14, 0x20, 0xF6, 0x40, 0x22, 0x99, 0xB3, 0x1A, 0x02, 0xD7, 0x3A};
	uint8_t iv[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t cipher[NB_BYTES_128_BITS];

	aes_count_iv(iv, cipher, NB_BYTES_128_BITS, key, plain);
	
	printf("Expected cipher (hex): "); print_uint8_array_as_hex_in_order(cipher_expected, NB_BYTES_128_BITS, true);
	printf("Actual cipher   (hex): "); print_uint8_array_as_hex_in_order(cipher, NB_BYTES_128_BITS, true);

	if(!compare_uint8_arrays(cipher, cipher_expected, NB_BYTES_128_BITS)) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test_nist2(){
	printf("######## Test NIST: test case 2 ########\n");

	uint8_t key[] = {0x00, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00};
	uint8_t plain[] = {0x00, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00};
	uint8_t cipher_expected[] = {0x03, 0x88, 0xda, 0xce, 0x60, 0xb6, 0xa3, 0x92, 0xf3, 0x28, 0xc2, 0xb9, 0x71, 0xb2, 0xfe, 0x78};
	uint8_t iv[] = {0x00, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x02};
	uint8_t cipher[NB_BYTES_128_BITS];

	aes_count_iv(plain, cipher, NB_BYTES_128_BITS, key, iv);
	
	printf("Expected cipher (hex): "); print_uint8_array_as_hex_in_order(cipher_expected, NB_BYTES_128_BITS, true);
	printf("Actual cipher   (hex): "); print_uint8_array_as_hex_in_order(cipher, NB_BYTES_128_BITS, true);

	if(!compare_uint8_arrays(cipher, cipher_expected, NB_BYTES_128_BITS)) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test_nist3(){
	printf("######## Test NIST: test case 3 ########\n");

	char key_str[] = "feffe9928665731c6d6a8f9467308308";
	char plain_str[] = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255";
	char iv_str[] = "cafebabefacedbaddecaf88800000002";
	// cc fe ba be fa ce db ad de ca f8 88 00 00 00 02
	// ccfedbabefacedbaddecaf8800000002
	// ca fe ba be fa ce db ad de ca f8 88 02 00 00 02 
	// cafebabefacedbaddecaf88020000002
	char cipher_expected_str[] = "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985";
	
	size_t plain_size = nb_bytes_hex_string(plain_str);
	size_t cipher_expected_size = nb_bytes_hex_string(cipher_expected_str);

	uint8_t key[NB_BYTES_128_BITS];
	uint8_t plain[plain_size];
	uint8_t iv[NB_BYTES_128_BITS];
	uint8_t cipher_expected[cipher_expected_size];
	convert_hex_string_to_uint8_array_in_order(key_str, key, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order(plain_str, plain, plain_size);
	convert_hex_string_to_uint8_array_in_order(iv_str, iv, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order(cipher_expected_str, cipher_expected, cipher_expected_size);

	uint8_t cipher[cipher_expected_size];
	aes_count_iv(plain, cipher, plain_size, key, iv);
	
	printf("Expected cipher (hex): "); print_uint8_array_as_hex_in_order(cipher_expected, cipher_expected_size, true);
	printf("Actual cipher   (hex): "); print_uint8_array_as_hex_in_order(cipher, cipher_expected_size, true);

	if(!compare_uint8_arrays(cipher, cipher_expected, cipher_expected_size)) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int main() {
	print_test_return_status(test_yoda());
	print_test_return_status(test_nist2());
	print_test_return_status(test_nist3());

	return CODE_SUCCESS;
}
