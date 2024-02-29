#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/utils.h"

extern void compute_hash_key_jazz(uint8_t* key, uint8_t* out_h);
extern void compute_length_str_jazz(size_t length_auth_data, size_t length_plain, uint8_t* ptrout_length_str);
extern void compute_enciphered_iv_jazz(uint8_t* key, uint8_t* iv, uint8_t* out_enc_iv);

extern void ghash_series_jazz(uint8_t* ptr_data, size_t length, uint8_t* hash_key, uint8_t* out_res, uint8_t* prev_ghash);

extern void aes_gcm(uint8_t** args, size_t length_auth_data, size_t length_plain);
extern uint64_t aes_gcm_inv(uint8_t** args, size_t length_auth_data, size_t length_cipher);



int test_nous() {
	printf("######## ultimate ca passe ou ca casse ########\n");

	uint8_t key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("feffe9928665731c6d6a8f9467308308", key, NB_BYTES_128_BITS);
	uint8_t iv[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("cafebabefacedbaddecaf88800000000", iv, NB_BYTES_128_BITS);
	char auth_data_str[] = "Heureusement tout finit bien";
	size_t length_auth_data = strlen(auth_data_str);
	uint8_t auth_data[length_auth_data]; convert_ascii_string_to_uint8_array_in_order(auth_data_str, auth_data, length_auth_data);
	char plain_str[] = "On est ravis de finir notre projet a temps";
	size_t length_plain = strlen(plain_str);
	uint8_t plain[length_plain]; convert_ascii_string_to_uint8_array_in_order(plain_str, plain, length_plain);

	uint8_t expected_auth_tag[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("91d999481bad6fc3cc7967ed7ab99f35", expected_auth_tag, NB_BYTES_128_BITS);
	char expected_cipher_str[] = "d4dc0c82aa8752b38f5d41010b4197260364e6154b163d556fff2b3ed3ef4436589d3846e17aff222233";
	uint8_t expected_cipher[length_plain]; convert_hex_string_to_uint8_array_in_order(expected_cipher_str, expected_cipher, length_plain);

	uint8_t auth_tag[NB_BYTES_128_BITS];
	uint8_t cipher[length_plain];

	// array of pointers
	uint8_t* args_func[] = { key, iv, auth_data, plain, auth_tag, cipher };
	aes_gcm(args_func, length_auth_data, length_plain);

	printf("Key                         (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("IV                          (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("Authentication data         (hex): "); print_uint8_array_as_ascii_in_order(auth_data, length_auth_data, false);
	printf("Plain                       (hex): "); print_uint8_array_as_ascii_in_order(plain, length_plain, false);
	printf("Expected authentication tag (hex): "); print_uint8_array_as_hex_in_order(expected_auth_tag, NB_BYTES_128_BITS, false);
	printf("Actual authentication tag   (hex): "); print_uint8_array_as_hex_in_order(auth_tag, NB_BYTES_128_BITS, false);
	printf("Expected cipher             (hex): "); print_uint8_array_as_hex_in_order(expected_cipher, length_plain, false);
	printf("Actual cipher               (hex): "); print_uint8_array_as_hex_in_order(cipher, length_plain, false);

	int return_code = CODE_SUCCESS;
	if (!compare_uint8_arrays(auth_tag, expected_auth_tag, NB_BYTES_128_BITS)) {
		printf("ERROR: Authentication tag is not as expected\n");
		return_code = CODE_FAILURE;
	}
	if (!compare_uint8_arrays(cipher, expected_cipher, length_plain)) {
		printf("ERROR: Cipher is not as expected\n");
		return_code = CODE_FAILURE;
	}
	return return_code;
}

int test_nist3() {
	printf("######## nist test case 3 ########\n");

	uint8_t key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("feffe9928665731c6d6a8f9467308308", key, NB_BYTES_128_BITS);
	uint8_t iv[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("cafebabefacedbaddecaf88800000000", iv, NB_BYTES_128_BITS);
	char auth_data_str[] = "";
	size_t length_auth_data = nb_bytes_hex_string(auth_data_str);
	uint8_t auth_data[length_auth_data]; convert_hex_string_to_uint8_array_in_order(auth_data_str, auth_data, length_auth_data);
	char plain_str[] = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255";
	size_t length_plain = nb_bytes_hex_string(plain_str);
	uint8_t plain[length_plain]; convert_hex_string_to_uint8_array_in_order(plain_str, plain, length_plain);

	uint8_t expected_auth_tag[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("4d5c2af327cd64a62cf35abd2ba6fab4", expected_auth_tag, NB_BYTES_128_BITS);
	char expected_cipher_str[] = "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985";
	uint8_t expected_cipher[length_plain]; convert_hex_string_to_uint8_array_in_order(expected_cipher_str, expected_cipher, length_plain);

	uint8_t auth_tag[NB_BYTES_128_BITS];
	uint8_t cipher[length_plain];

	// array of pointers
	uint8_t* args_func[] = { key, iv, auth_data, plain, auth_tag, cipher };
	aes_gcm(args_func, length_auth_data, length_plain);

	printf("Key                         (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("IV                          (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("Authentication data         (hex): "); print_uint8_array_as_hex_in_order(auth_data, length_auth_data, false);
	printf("Plain                       (hex): "); print_uint8_array_as_hex_in_order(plain, length_plain, false);
	printf("Expected authentication tag (hex): "); print_uint8_array_as_hex_in_order(expected_auth_tag, NB_BYTES_128_BITS, false);
	printf("Actual authentication tag   (hex): "); print_uint8_array_as_hex_in_order(auth_tag, NB_BYTES_128_BITS, false);
	printf("Expected cipher             (hex): "); print_uint8_array_as_hex_in_order(expected_cipher, length_plain, false);
	printf("Actual cipher               (hex): "); print_uint8_array_as_hex_in_order(cipher, length_plain, false);

	int return_code = CODE_SUCCESS;
	if (!compare_uint8_arrays(auth_tag, expected_auth_tag, NB_BYTES_128_BITS)) {
		printf("ERROR: Authentication tag is not as expected\n");
		return_code = CODE_FAILURE;
	}
	if (!compare_uint8_arrays(cipher, expected_cipher, length_plain)) {
		printf("ERROR: Cipher is not as expected\n");
		return_code = CODE_FAILURE;
	}
	return return_code;
}
int test_nist4() {
	printf("######## nist test case 4 ########\n");

	uint8_t key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("feffe9928665731c6d6a8f9467308308", key, NB_BYTES_128_BITS);
	uint8_t iv[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("cafebabefacedbaddecaf88800000000", iv, NB_BYTES_128_BITS);
	char auth_data_str[] = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
	size_t length_auth_data = nb_bytes_hex_string(auth_data_str);
	uint8_t auth_data[length_auth_data]; convert_hex_string_to_uint8_array_in_order(auth_data_str, auth_data, length_auth_data);
	char plain_str[] = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39";
	size_t length_plain = nb_bytes_hex_string(plain_str);
	uint8_t plain[length_plain]; convert_hex_string_to_uint8_array_in_order(plain_str, plain, length_plain);

	uint8_t expected_auth_tag[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("5bc94fbc3221a5db94fae95ae7121a47", expected_auth_tag, NB_BYTES_128_BITS);
	char expected_cipher_str[] = "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091";
	uint8_t expected_cipher[length_plain]; convert_hex_string_to_uint8_array_in_order(expected_cipher_str, expected_cipher, length_plain);

	uint8_t auth_tag[NB_BYTES_128_BITS];
	uint8_t cipher[length_plain];

	// array of pointers
	uint8_t* args_func[] = { key, iv, auth_data, plain, auth_tag, cipher };
	aes_gcm(args_func, length_auth_data, length_plain);

	printf("Key                         (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("IV                          (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("Authentication data         (hex): "); print_uint8_array_as_hex_in_order(auth_data, length_auth_data, false);
	printf("Plain                       (hex): "); print_uint8_array_as_hex_in_order(plain, length_plain, false);
	printf("Expected authentication tag (hex): "); print_uint8_array_as_hex_in_order(expected_auth_tag, NB_BYTES_128_BITS, false);
	printf("Actual authentication tag   (hex): "); print_uint8_array_as_hex_in_order(auth_tag, NB_BYTES_128_BITS, false);
	printf("Expected cipher             (hex): "); print_uint8_array_as_hex_in_order(expected_cipher, length_plain, false);
	printf("Actual cipher               (hex): "); print_uint8_array_as_hex_in_order(cipher, length_plain, false);

	int return_code = CODE_SUCCESS;
	if (!compare_uint8_arrays(auth_tag, expected_auth_tag, NB_BYTES_128_BITS)) {
		printf("ERROR: Authentication tag is not as expected\n");
		return_code = CODE_FAILURE;
	}
	if (!compare_uint8_arrays(cipher, expected_cipher, length_plain)) {
		printf("ERROR: Cipher is not as expected\n");
		return_code = CODE_FAILURE;
	}
	return return_code;
}

int test_nist3_inv() {
	printf("######## nist test case 3 inv ########\n");

	uint8_t key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("feffe9928665731c6d6a8f9467308308", key, NB_BYTES_128_BITS);
	uint8_t iv[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("cafebabefacedbaddecaf88800000000", iv, NB_BYTES_128_BITS);
	char auth_data_str[] = "";
	size_t length_auth_data = nb_bytes_hex_string(auth_data_str);
	uint8_t auth_data[length_auth_data]; convert_hex_string_to_uint8_array_in_order(auth_data_str, auth_data, length_auth_data);
	char cipher_str[] = "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985";
	size_t length_cipher = nb_bytes_hex_string(cipher_str);
	uint8_t cipher[length_cipher]; convert_hex_string_to_uint8_array_in_order(cipher_str, cipher, length_cipher);

	uint8_t expected_auth_tag[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("4d5c2af327cd64a62cf35abd2ba6fab4", expected_auth_tag, NB_BYTES_128_BITS);
	char expected_plain_str[] = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255";
	uint8_t expected_plain[length_cipher]; convert_hex_string_to_uint8_array_in_order(expected_plain_str, expected_plain, length_cipher);

	uint8_t plain[length_cipher];

	// array of pointers
	uint8_t* args_func[] = { key, iv, auth_data, cipher, expected_auth_tag, plain };
	uint64_t gcm_status = aes_gcm_inv(args_func, length_auth_data, length_cipher);

	printf("Key                         (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("IV                          (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("Authentication data         (hex): "); print_uint8_array_as_hex_in_order(auth_data, length_auth_data, false);
	printf("Cipher                      (hex): "); print_uint8_array_as_hex_in_order(cipher, length_cipher, false);
	printf("exprected authentication tag(hex): "); print_uint8_array_as_hex_in_order(expected_auth_tag, NB_BYTES_128_BITS, false);
	printf("Expected plain              (hex): "); print_uint8_array_as_hex_in_order(expected_plain, length_cipher, false);
	printf("Actual plain                (hex): "); print_uint8_array_as_hex_in_order(plain, length_cipher, false);

	int return_code = CODE_SUCCESS;
	if (gcm_status != 0) {
		printf("ERROR: Tag verification failed\n");
		return_code = CODE_FAILURE;
	}
	if (!compare_uint8_arrays(plain, expected_plain, length_cipher)) {
		printf("ERROR: Plain is not as expected\n");
		return_code = CODE_FAILURE;
	}
	return return_code;
}

int test_nist4_inv() {
	printf("######## nist test case 4 inv ########\n");

	uint8_t key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("feffe9928665731c6d6a8f9467308308", key, NB_BYTES_128_BITS);
	uint8_t iv[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("cafebabefacedbaddecaf88800000000", iv, NB_BYTES_128_BITS);
	char auth_data_str[] = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
	size_t length_auth_data = nb_bytes_hex_string(auth_data_str);
	uint8_t auth_data[length_auth_data]; convert_hex_string_to_uint8_array_in_order(auth_data_str, auth_data, length_auth_data);
	char cipher_str[] = "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091";
	size_t length_cipher = nb_bytes_hex_string(cipher_str);
	uint8_t cipher[length_cipher]; convert_hex_string_to_uint8_array_in_order(cipher_str, cipher, length_cipher);

	uint8_t auth_tag[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("5bc94fbc3221a5db94fae95ae7121a47", auth_tag, NB_BYTES_128_BITS);
	char expected_plain_str[] = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39";
	uint8_t expected_plain[length_cipher]; convert_hex_string_to_uint8_array_in_order(expected_plain_str, expected_plain, length_cipher);

	uint8_t plain[length_cipher];

	// array of pointers
	uint8_t* args_func[] = { key, iv, auth_data, cipher, auth_tag, plain };
	uint64_t gcm_status = aes_gcm_inv(args_func, length_auth_data, length_cipher);

	printf("Key                         (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("IV                          (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("Authentication data         (hex): "); print_uint8_array_as_hex_in_order(auth_data, length_auth_data, false);
	printf("Cipher                      (hex): "); print_uint8_array_as_hex_in_order(cipher, length_cipher, false);
	printf("authentication tag          (hex): "); print_uint8_array_as_hex_in_order(auth_tag, NB_BYTES_128_BITS, false);
	printf("Expected plain              (hex): "); print_uint8_array_as_hex_in_order(expected_plain, length_cipher, false);
	printf("Actual plain                (hex): "); print_uint8_array_as_hex_in_order(plain, length_cipher, false);

	int return_code = CODE_SUCCESS;
	if (gcm_status != 0) {
		printf("ERROR: Tag verification failed\n");
		return_code = CODE_FAILURE;
	}
	if (!compare_uint8_arrays(plain, expected_plain, length_cipher)) {
		printf("ERROR: Plain is not as expected\n");
		return_code = CODE_FAILURE;
	}
	return return_code;
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

int test_compute_length_str_nist4() {
	printf("######## test_compute_length_str nist 4 ########\n");

	char auth_data_str[] = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
	size_t length_auth_data = nb_bytes_hex_string(auth_data_str);
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

int test_compute_enciphered_iv_nist3() {
	printf("######## test_compute_enciphered_iv_nist3 ########\n");

	uint8_t key[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("feffe9928665731c6d6a8f9467308308", key, NB_BYTES_128_BITS);
	uint8_t iv[NB_BYTES_128_BITS];  convert_hex_string_to_uint8_array_in_order("cafebabefacedbaddecaf88854000054", iv, NB_BYTES_128_BITS);

	uint8_t enc_iv[NB_BYTES_128_BITS]; compute_enciphered_iv_jazz(key, iv, enc_iv);

	printf("Key           (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("IV            (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("Enciphered IV (hex): "); print_uint8_array_as_hex_in_order(enc_iv, NB_BYTES_128_BITS, false);

	return CODE_INFO;
}

int demo() {
	printf("######## AES-GCM Encryption demo ########\n");

	uint8_t key[NB_BYTES_128_BITS]; convert_ascii_string_to_uint8_array_in_order("Banane et orange", key, NB_BYTES_128_BITS);
	uint8_t iv[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("abcdef0123456789abcdef4200000000", iv, NB_BYTES_128_BITS);
	char auth_data_str[] = "Ceci est un message qui servira à créer le tag d'authentification.";
	size_t length_auth_data = strlen(auth_data_str);
	uint8_t auth_data[length_auth_data]; convert_ascii_string_to_uint8_array_in_order(auth_data_str, auth_data, length_auth_data);
	char plain_str[] = "Et voilà le message qui sera chiffré grâce à AES-GCM.";
	size_t length_plain = strlen(plain_str);
	uint8_t plain[length_plain]; convert_ascii_string_to_uint8_array_in_order(plain_str, plain, length_plain);

	uint8_t auth_tag[NB_BYTES_128_BITS];
	uint8_t cipher[length_plain];

	uint8_t* args_func[] = { key, iv, auth_data, plain, auth_tag, cipher };
	aes_gcm(args_func, length_auth_data, length_plain);

	
	printf("INPUTS\n");
	printf("\tKey                 (ascii): "); print_uint8_array_as_ascii_in_order(key, NB_BYTES_128_BITS, false);
	printf("\tKey                   (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("\tIV                    (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("\tAuthentication data (ascii): "); print_uint8_array_as_ascii_in_order(auth_data, length_auth_data, false);
	printf("\tAuthentication data   (hex): "); print_uint8_array_as_hex_in_order(auth_data, length_auth_data, false);
	printf("\tPlain               (ascii): "); print_uint8_array_as_ascii_in_order(plain, length_plain, false);
	printf("\tPlain                 (hex): "); print_uint8_array_as_hex_in_order(plain, length_plain, false);

	printf("\nOUTPUTS\n");
	printf("\tAuthentication tag    (hex): "); print_uint8_array_as_hex_in_order(auth_tag, NB_BYTES_128_BITS, false);
	printf("\tCipher                (hex): "); print_uint8_array_as_hex_in_order(cipher, length_plain, false);



	printf("\n######## AES-GCM Decryption demo ########\n");

	char cipher_str2[] = "5115e98435231141fc181d02df630d7543c412fe61a889631a48ea5f4035b2f8ba61539767e38c3fad196ffffc817960cdb851694b935a0cc9";
	size_t length_cipher2 = nb_bytes_hex_string(cipher_str2);
	uint8_t cipher2[length_cipher2]; convert_hex_string_to_uint8_array_in_order(cipher_str2, cipher2, length_cipher2);
	uint8_t auth_tag2[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array_in_order("11169a66e6e780eebc741eda7d0b4142", auth_tag2, NB_BYTES_128_BITS);

	uint8_t plain2[length_cipher2];

	uint8_t* args_func2[] = { key, iv, auth_data, cipher2, auth_tag2, plain2 };
	uint64_t gcm_status = aes_gcm_inv(args_func2, length_auth_data, length_cipher2);

	
	printf("INPUTS\n");
	printf("\tKey                 (ascii): "); print_uint8_array_as_ascii_in_order(key, NB_BYTES_128_BITS, false);
	printf("\tKey                   (hex): "); print_uint8_array_as_hex_in_order(key, NB_BYTES_128_BITS, false);
	printf("\tIV                    (hex): "); print_uint8_array_as_hex_in_order(iv, NB_BYTES_128_BITS, false);
	printf("\tAuthentication data (ascii): "); print_uint8_array_as_ascii_in_order(auth_data, length_auth_data, false);
	printf("\tAuthentication data   (hex): "); print_uint8_array_as_hex_in_order(auth_data, length_auth_data, false);
	printf("\tCipher                (hex): "); print_uint8_array_as_hex_in_order(cipher2, length_cipher2, false);

	printf("\nOUTPUTS\n");
	printf("\tPlain                 (hex): "); print_uint8_array_as_hex_in_order(plain2, length_cipher2, false);
	printf("\tPlain               (ascii): "); print_uint8_array_as_ascii_in_order(plain2, length_cipher2, false);
	printf("\tAuthentication tag verification status: "); if (gcm_status == 0) { printf("SUCCESS\n"); } else { printf("FAILURE\n"); }


	return CODE_INFO;
}

int main()
{
	print_test_return_status(test_compute_hash_key_nist2());
	print_test_return_status(test_compute_hash_key_nist4());

	print_test_return_status(test_compute_length_str_nist4());

	print_test_return_status(test_compute_enciphered_iv_nist3());

	print_test_return_status(test_nist3());
	print_test_return_status(test_nist3_inv());
	print_test_return_status(test_nist4());
	print_test_return_status(test_nist4_inv());
	print_test_return_status(test_nous());

	print_test_return_status(demo());

	return CODE_SUCCESS;
}
