#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/utils.h"

// extern void aes_gcm_jazz(uint8_t* key, uint8_t* iv, uint8_t* auth_data, size_t length_auth_data, uint8_t* plain, size_t length_plain, uint8_t* out_auth_tag, uint8_t* out_cipher);
extern void compute_hash_key_jazz(uint8_t* key, uint8_t* out_h);
extern void compute_length_str_jazz(size_t length_auth_data, size_t length_plain, uint8_t* ptrout_length_str);
extern void aes_gcm(uint8_t* args);

int test_nist4() {
	printf("######## nist test case 4 ########\n");

	uint8_t key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("feffe9928665731c6d6a8f9467308308", key, NB_BYTES_128_BITS);
	uint8_t iv[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("cafebabefacedbaddecaf88800000000", iv, NB_BYTES_128_BITS);
	uint8_t auth_data[] = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
	size_t length_auth_data = nb_bytes_hex_string(auth_data);
	char plain_str[] = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39";
	size_t length_plain = nb_bytes_hex_string(plain_str);
	uint8_t plain[length_plain]; convert_hex_string_to_uint8_array_in_order(plain_str, plain, length_plain);

	uint8_t expected_auth_tag[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("5bc94fbc3221a5db94fae95ae7121a47", expected_auth_tag, NB_BYTES_128_BITS);
	char expected_cipher_str[] = "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091";
	uint8_t expected_cipher[length_plain]; convert_hex_string_to_uint8_array_in_order(expected_cipher_str, expected_cipher, length_plain);

	uint8_t auth_tag[NB_BYTES_128_BITS];
	uint8_t cipher[length_plain];

	// aes_gcm_jazz(key, iv, auth_data, length_auth_data, plain, length_plain, auth_tag, cipher);
	// array of pointers
	uint8_t* args_func[] = {key, iv, auth_data, length_auth_data, plain, length_plain, auth_tag, cipher};
	aes_gcm(args_func);

	printf("Key                         (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("IV                          (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("Authentication data         (hex): "); print_uint8_array_as_hex_in_order(auth_data, length_auth_data, false);
	printf("Plain                       (hex): "); print_uint8_array_as_hex_in_order(plain, length_plain, false);
	printf("Expected authentication tag (hex): "); print_uint8_array_as_hex_in_order(expected_auth_tag, NB_BYTES_128_BITS, false);
	printf("Actual authentication tag   (hex): "); print_uint8_array_as_hex_in_order(auth_tag, NB_BYTES_128_BITS, false);
	printf("Expected cipher             (hex): "); print_uint8_array_as_hex_in_order(expected_cipher, length_plain, false);
	printf("Actual cipher               (hex): "); print_uint8_array_as_hex_in_order(cipher, length_plain, false);

	if (!compare_uint8_arrays(auth_tag, expected_auth_tag, NB_BYTES_128_BITS)) {
		printf("ERROR: Authentication tag is not as expected\n");
		return CODE_FAILURE;
	}
	if (!compare_uint8_arrays(cipher, expected_cipher, length_plain)) {
		printf("ERROR: Cipher is not as expected\n");
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_compute_hash_key_generic(char* key_str, char* expected_hash_key_str) {
	uint8_t key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order(key_str, key, NB_BYTES_128_BITS);
	uint8_t expected_hash_key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order(expected_hash_key_str, expected_hash_key, NB_BYTES_128_BITS);

	uint8_t hash_key[NB_BYTES_128_BITS]; compute_hash_key_jazz(key, hash_key);

	printf("Key               (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("Expected hash key (hex): "); print_uint8_array_as_hex_in_order(expected_hash_key, NB_BYTES_128_BITS, false);
	printf("Actual hash key   (hex): "); print_uint8_array_as_hex_in_order(hash_key, NB_BYTES_128_BITS, false);

	if (!compare_uint8_arrays(hash_key, expected_hash_key, NB_BYTES_128_BITS)) {
		printf("ERROR: hash key is not as expected\n");
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_compute_hash_key_nist2() {
	printf("######## test_compute_hash_key nist test case 2 ########\n");
	return test_compute_hash_key_generic("00000000000000000000000000000000", "66e94bd4ef8a2c3b884cfa59ca342b2e");
}

int test_compute_hash_key_nist4() {
	printf("######## test_compute_hash_key nist test case 4 ########\n");
	return test_compute_hash_key_generic("feffe9928665731c6d6a8f9467308308", "b83b533708bf535d0aa6e52980d53b78");
}

int test_compute_length_str_nist_4() {
	printf("######## test_compute_length_str nist 4 ########\n");

	uint8_t auth_data[] = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
	size_t length_auth_data = nb_bytes_hex_string(auth_data);
	char plain_str[] = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39";
	size_t length_plain = nb_bytes_hex_string(plain_str);
	uint8_t expected_length_str[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("00000000000000a000000000000001e0", expected_length_str, NB_BYTES_128_BITS);

	uint8_t length_str[NB_BYTES_128_BITS]; compute_length_str_jazz(length_auth_data, length_plain, length_str);

	printf("Length of authentication data: dec=%zu, hex=0x%zx\n", length_auth_data*8, length_auth_data*8);
	printf("Length of plain              : dec=%zu, hex=0x%zx\n", length_plain*8, length_plain*8);
	printf("Length string           (hex): "); print_uint8_array_as_hex_in_order(length_str, NB_BYTES_128_BITS, false);
	printf("Expected length string  (hex): "); print_uint8_array_as_hex_in_order(expected_length_str, NB_BYTES_128_BITS, false);

	if (!compare_uint8_arrays(length_str, expected_length_str, NB_BYTES_128_BITS)) {
		printf("ERROR: length string is not as expected\n");
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int main()
{
	print_test_return_status(test_nist4());

	print_test_return_status(test_compute_hash_key_nist2());
	print_test_return_status(test_compute_hash_key_nist4());

	print_test_return_status(test_compute_length_str_nist_4());

	return CODE_SUCCESS;
}
