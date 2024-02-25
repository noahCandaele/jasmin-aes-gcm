/*******************************************************************
 * This code is adapted from Vincent LAPORTE, Cyber In Nancy 2022: *
 * https://members.loria.fr/VLaporte/files/CyberIn2022.html        *
 *******************************************************************/

#include <smmintrin.h>
#include <immintrin.h>
#include <sys/types.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "../../utils/utils.h"

extern __m128i aes_jazz(__m128i key, __m128i plain);
extern __m128i invaes_jazz(__m128i key, __m128i cipher);

extern __m128i aes_block_cipher_mode_jazz(__m128i key, __m128i counter, __m128i plain);
extern __m128i invaes_block_cipher_mode_jazz(__m128i key, __m128i counter, __m128i cipher);

__m128i arr_to_u128(int8_t* arr) {
	return _mm_loadu_si128((__m128i *) arr);
}

void u128_to_arr(__m128i value, int8_t* arr) {
	_mm_storeu_si128((__m128i *)arr, value);
}

static int test_cpu_compatibility(void) {
	printf("######## Checking CPU compatibility ########\n");
	return check_cpu_compatibility();
}

void aes(uint8_t* key, uint8_t* plain, uint8_t* cipher_computed) {
	u128_to_arr(aes_jazz(arr_to_u128(key), arr_to_u128(plain)), cipher_computed);
}

void invaes(uint8_t* key, uint8_t* cipher, uint8_t* plain_computed) {
	u128_to_arr(invaes_jazz(arr_to_u128(key), arr_to_u128(cipher)), plain_computed);
}

void aes_block_cipher_mode(uint8_t* key, uint8_t* counter, uint8_t* plain, uint8_t* cipher_computed) {
	u128_to_arr(aes_block_cipher_mode_jazz(arr_to_u128(key), arr_to_u128(counter), arr_to_u128(plain)), cipher_computed);
}

void invaes_block_cipher_mode(uint8_t* key, uint8_t* counter, uint8_t* cipher, uint8_t* plain_computed) {
	u128_to_arr(aes_block_cipher_mode_jazz(arr_to_u128(key), arr_to_u128(counter), arr_to_u128(cipher)), plain_computed);
}

void generate_iv(uint8_t* iv) {
	bool loop = true;
	// create first 64-bit part of the IV
	unsigned long long iv_part1;
	while (loop) {
		__builtin_ia32_rdrand64_step(&iv_part1);
		// verify that the first 64-bit part of the IV is not zero
		if (iv_part1 > 0xFFFFFFFFFFFFFFF) {
			loop = false;
		}
	}
	loop = true;
	uint32_t iv_part2;
	while (loop) {
		// create second 32-bit part of the IV
		__builtin_ia32_rdrand32_step(&iv_part2);
		if (iv_part2 > 0xFFFFFFF) {
			loop = false;
		}
	}

	uint32_t zeros = 0;

	char iv_char_arr[sizeof(iv_part1) * 2 + sizeof(iv_part2) * 2 + sizeof(zeros) * 2 + 1];
    sprintf(iv_char_arr, "%llx%x%08x", iv_part1, iv_part2, zeros);
	convert_hex_string_to_uint8_array(iv_char_arr, iv, NB_BYTES_128_BITS);
}

int test_aes128() {
	printf("######## Test AES-128 ########\n");

	printf("Test 1\n");
	
	uint8_t* key = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* plain = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* cipher = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (key == NULL || plain == NULL || cipher == NULL) {
		printf("test_aes128: allocation of key, plain or cipher failed");
		return CODE_FAILURE;
	}
	convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("29C3505F571420F6402299B31A02D73A", cipher, NB_BYTES_128_BITS);

	printf("key (hex)     : "); print_uint8_array_as_hex(key, NB_BYTES_128_BITS, true);
	printf("key (ascii)   : "); print_uint8_array_as_ascii(key, NB_BYTES_128_BITS, true);
	printf("\n");
	printf("plain (hex)   : "); print_uint8_array_as_hex(plain, NB_BYTES_128_BITS, true);
	printf("plain (ascii) : "); print_uint8_array_as_ascii(plain, NB_BYTES_128_BITS, true);
	printf("\n");
	printf("cipher (hex)  : "); print_uint8_array_as_hex(cipher, NB_BYTES_128_BITS, true);
	printf("cipher (ascii): "); print_uint8_array_as_ascii(cipher, NB_BYTES_128_BITS, true);
	printf("\n");

	printf("Jasmin-generated encryption\n");
	uint8_t* cipher_computed = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (cipher_computed == NULL) {
		printf("test_aes128: allocation of cipher_computed failed");
		return CODE_FAILURE;
	}
	aes(key, plain, cipher_computed);
	printf("computed cipher (hex)  : "); print_uint8_array_as_hex(cipher_computed, NB_BYTES_128_BITS, true);
	printf("computed cipher (ascii): "); print_uint8_array_as_ascii(cipher_computed, NB_BYTES_128_BITS, true);
	printf("\n");

	printf("Jasmin-generated decryption\n");
	uint8_t* plain_computed = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (plain_computed == NULL) {
		printf("test_aes128: allocation of plain_computed failed");
		return CODE_FAILURE;
	}
	invaes(key, cipher_computed, plain_computed);
	printf("computed plain (hex)  : "); print_uint8_array_as_hex(plain_computed, NB_BYTES_128_BITS, true);
	printf("computed plain (ascii): "); print_uint8_array_as_ascii(plain_computed, NB_BYTES_128_BITS, true);
	printf("\n");

	// Compare expected values with computed values
	bool plain_success = compare_uint8_arrays(plain, plain_computed, NB_BYTES_128_BITS);
	if(plain_success) {
		printf("Success: plain has been computed as expected\n");
	} else {
		printf("Failure: plain has not been computed as expected\n");
		return CODE_FAILURE;
	}
	bool cipher_success = compare_uint8_arrays(cipher, cipher_computed, NB_BYTES_128_BITS);
	if(cipher_success) {
		printf("Success: cipher has been computed as expected\n");
	} else {
		printf("Failure: cipher has not been computed as expected\n");
		return CODE_FAILURE;
	}

	free(key); free(plain); free(cipher); free(cipher_computed); free(plain_computed);

	// #######################################################
	printf("\n");
	printf("Test 2\n");

	uint8_t key2[]    = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	uint8_t plain2[]  = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	uint8_t cipher2[] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};

	uint8_t* cipher_computed2 = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (cipher_computed2 == NULL) {
		printf("test_aes128: allocation of cipher_computed2 failed");
		return CODE_FAILURE;
	}
	aes(key2, plain2, cipher_computed2);

	uint8_t* plain_computed2 = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (plain_computed2 == NULL) {
		printf("test_aes128: allocation of plain_computed2 failed");
		return CODE_FAILURE;
	}
	invaes(key2, cipher_computed2, plain_computed2);

	// Compare expected values with computed values
	bool plain_success2 = compare_uint8_arrays(plain2, plain_computed2, NB_BYTES_128_BITS);
	if(plain_success2) {
		printf("Success: plain2 has been computed as expected\n");
	} else {
		printf("Failure: plain2 has not been computed as expected\n");
		return CODE_FAILURE;
	}
	bool cipher_success2 = compare_uint8_arrays(cipher2, cipher_computed2, NB_BYTES_128_BITS);
	if(cipher_success2) {
		printf("Success: cipher2 has been computed as expected\n");
	} else {
		printf("Failure: cipher2 has not been computed as expected\n");
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_aes256() {
	printf("######## Test AES-256 ########\n");
	
	uint8_t* key = (uint8_t*)malloc(NB_BYTES_256_BITS * sizeof(uint8_t));
	uint8_t* plain = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* cipher = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (key == NULL || plain == NULL || cipher == NULL) {
		printf("test_aes256: allocation of key, plain or cipher failed");
		return CODE_FAILURE;
	}
	convert_hex_string_to_uint8_array("c47b0294dbbbee0fec4757f22ffeee3587ca4730c3d33b691df38bab076bc558", key, NB_BYTES_256_BITS);
	convert_hex_string_to_uint8_array("00000000000000000000000000000000", plain, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("46f2fb342d6f0ab477476fc501242c5f", cipher, NB_BYTES_128_BITS);

	free(key); free(plain); free(cipher);

	// TODO test not fully implemented
	return CODE_FAILURE;
}

int test_aes128_block_cipher_mode_encryption_module() {
	printf("######## Test AES block cipher mode of operation, encryption part, 1 module ########\n");

	printf("Test 1\n");

	uint8_t* key = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* iv = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* plain = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* cipher = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (key == NULL || iv == NULL || plain == NULL || cipher == NULL) {
		printf("test_aes128_block_cipher_mode: allocation of key, iv, plain or cipher failed");
		return CODE_FAILURE;
	}
	// TODO remettre
	convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("f34481ec3cc627bacd5dc3fb08f273e6", iv, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("479be376295fc8547276c4c9a76d2822", cipher, NB_BYTES_128_BITS);

	printf("Jasmin-generated encryption\n");
	uint8_t* cipher_computed = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (cipher_computed == NULL) {
		printf("test_aes128: allocation of cipher_computed failed");
		return CODE_FAILURE;
	}
	aes_block_cipher_mode(key, iv, plain, cipher_computed);
	printf("computed cipher (hex)  : "); print_uint8_array_as_hex(cipher_computed, NB_BYTES_128_BITS, true);
	printf("computed cipher (ascii): "); print_uint8_array_as_ascii(cipher_computed, NB_BYTES_128_BITS, true);
	printf("\n");

	bool cipher_success = compare_uint8_arrays(cipher, cipher_computed, NB_BYTES_128_BITS);
	if(cipher_success) {
		printf("Success: cipher has been computed as expected\n");
	} else {
		printf("Failure: cipher has not been computed as expected\n");
		return CODE_FAILURE;
	}

	free(key); free(iv); free(plain); free(cipher); free(cipher_computed);

	// #########################################################
	printf("\n");
	printf("Test 2: compute IV\n");

	uint8_t* key2 = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* iv2 = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* plain2 = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (key2 == NULL || iv2 == NULL || plain2 == NULL) {
		printf("test_aes128_block_cipher_mode: allocation of key2, plain2 failed");
		return CODE_FAILURE;
	}
	convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key2, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain2, NB_BYTES_128_BITS);

	generate_iv(iv2);

    printf("iv2: ");
	print_uint8_array_as_hex(iv2, NB_BYTES_128_BITS, true);

	printf("Jasmin-generated encryption\n");
	uint8_t* cipher_computed2 = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (cipher_computed2 == NULL) {
		printf("test_aes128_block_cipher_mode: allocation of cipher_computed2 failed");
		return CODE_FAILURE;
	}
	aes_block_cipher_mode(key2, iv2, plain2, cipher_computed2);
	printf("computed cipher (hex)  : "); print_uint8_array_as_hex(cipher_computed2, NB_BYTES_128_BITS, true);
	printf("computed cipher (ascii): "); print_uint8_array_as_ascii(cipher_computed2, NB_BYTES_128_BITS, true);
	printf("\n");

	printf("Jasmin-generated decryption\n");
	uint8_t* plain_computed2 = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (plain_computed2 == NULL) {
		printf("test_aes128_block_cipher_mode: allocation of plain_computed2 failed");
		return CODE_FAILURE;
	}
	invaes_block_cipher_mode(key2, iv2, cipher_computed2, plain_computed2);
	printf("computed plain (hex)  : "); print_uint8_array_as_hex(plain_computed2, NB_BYTES_128_BITS, true);
	printf("computed plain (ascii): "); print_uint8_array_as_ascii(plain_computed2, NB_BYTES_128_BITS, true);
	printf("\n");

	// Compare expected values with computed values
	bool plain_success2 = compare_uint8_arrays(plain2, plain_computed2, NB_BYTES_128_BITS);
	if(plain_success2) {
		printf("Success: plain has been computed as expected\n");
	} else {
		printf("Failure: plain has not been computed as expected\n");
		return CODE_FAILURE;
	}

	free(key2); free(iv2); free(plain2); free(cipher_computed2); free(plain_computed2);

	return CODE_SUCCESS;
}

int test_aes128_block_cipher_mode_encryption_full() {
	printf("######## Test AES block cipher mode of operation, encryption part, full ########\n");

	// char* plaintext_full = "On teste avec ca, on va voir si ca marche, si ca marche ca marche, si ca marche pas ca marche pas.";
	char* plaintext_full = "lorem ipsum dol";
	// char* plaintext_full = "efghijklmnopqrstuvwxyz0123456789";
	// char* plaintext_full = "efghijklmnopqrst";


	size_t len = strlen(plaintext_full);
    // char* plaintext_hex_full = (char*)malloc(len * 2 + 1); // Each character will be represented by 2 hex digits, plus 1 for null terminator
	uint8_t* plaintext_hex_full = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	// string_to_hex(plaintext_full, plaintext_hex_full);
	

	convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plaintext_hex_full, NB_BYTES_128_BITS);
	printf("plaintext full hex: "); print_uint8_array_as_hex(plaintext_hex_full, strlen(plaintext_hex_full), true);
	
	uint8_t* key = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* iv = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	uint8_t* counter = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
	if (key == NULL || iv == NULL || counter == NULL) {
		printf("test_aes128_block_cipher_mode_encryption_full: allocation failed");
		return CODE_FAILURE;
	}
	convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// generate_iv(iv);
	convert_hex_string_to_uint8_array("f34481ec3cc627bacd5dc3fb08f273e6", iv, NB_BYTES_128_BITS);

	// convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array("f34481ec3cc627bacd5dc3fb08f273e6", iv, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array("479be376295fc8547276c4c9a76d2822", cipher, NB_BYTES_128_BITS);

	// list of cipher blocks
	int nb_cipher_blocks = ceil(strlen(plaintext_hex_full) / 16.0);
	printf("strlen(plaintext_full): %zu\n", strlen(plaintext_hex_full));
	printf("nb_cipher_blocks: %d\n", nb_cipher_blocks);
	uint8_t* cipher_blocks = (uint8_t*)malloc(nb_cipher_blocks * NB_BYTES_128_BITS * sizeof(uint8_t));
	if (cipher_blocks == NULL) {
		printf("test_aes128_block_cipher_mode_encryption_full: allocation failed");
		return CODE_FAILURE;
	}
	// for each 128 bit block of the plaintext
	for (size_t i = 0; i < nb_cipher_blocks; i++) {
		uint8_t* plain = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
		uint8_t* cipher = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
		if (plain == NULL || cipher == NULL) {
			printf("test_aes128_block_cipher_mode_encryption_full: allocation failed");
			return CODE_FAILURE;
		}
		// get ith block of plaintext // TODO mistake here
		convert_hex_string_to_uint8_array(plaintext_hex_full + i * NB_BYTES_128_BITS*2*sizeof(char), plain, NB_BYTES_128_BITS);

		// print plain
		printf("plain block %zu (hex)  : ", i);
		print_uint8_array_as_hex(plain, NB_BYTES_128_BITS, true); 
		aes_block_cipher_mode(key, iv, plain, cipher);
		// add cipher to list of cipher blocks
		memcpy(cipher_blocks + i * NB_BYTES_128_BITS*sizeof(uint8_t), cipher, NB_BYTES_128_BITS);
		free(plain); free(cipher);
	}
	// print cipher blocks
	for (size_t i = 0; i < nb_cipher_blocks; ++i) {
		printf("cipher block %zu: ", i);
		print_uint8_array_as_hex(cipher_blocks + i * NB_BYTES_128_BITS*sizeof(uint8_t), NB_BYTES_128_BITS, true);
	}
	// decrypt cipher blocks
	uint8_t* plain_computed_full = (uint8_t*)malloc(nb_cipher_blocks*NB_BYTES_128_BITS * sizeof(uint8_t));
	for (size_t i = 0; i < nb_cipher_blocks; ++i) {
		uint8_t* plain_computed = (uint8_t*)malloc(NB_BYTES_128_BITS * sizeof(uint8_t));
		if (plain_computed == NULL) {
			printf("test_aes128_block_cipher_mode_encryption_full: allocation failed");
			return CODE_FAILURE;
		}
		invaes_block_cipher_mode(key, iv, cipher_blocks + i * NB_BYTES_128_BITS, plain_computed);
		memcpy(plain_computed_full + i * NB_BYTES_128_BITS, plain_computed, NB_BYTES_128_BITS);
		free(plain_computed);
	}
	// print plain computed
	for (size_t i = 0; i < nb_cipher_blocks; ++i) {
		printf("plain computed block %zu (hex)  : ", i);
		print_uint8_array_as_hex(plain_computed_full + i * NB_BYTES_128_BITS, NB_BYTES_128_BITS, true);
		printf("plain computed block %zu (ascii): ", i);
		print_uint8_array_as_ascii(plain_computed_full + i * NB_BYTES_128_BITS, NB_BYTES_128_BITS, true);
	}
	// convert plain computed to string
	printf("full plaintext (hex)  : "); print_uint8_array_as_hex(plain_computed_full, nb_cipher_blocks*NB_BYTES_128_BITS, true);
	printf("full plaintext (ascii): "); print_uint8_array_as_ascii(plain_computed_full, nb_cipher_blocks*NB_BYTES_128_BITS, true);
	
	// TODO free tout
	// TODO test pas complet
	return CODE_FAILURE;
}

int main() {
	print_test_return_status(test_cpu_compatibility());

	print_test_return_status(test_aes128());
	// print_test_return_status(test_aes256());
	print_test_return_status(test_aes128_block_cipher_mode_encryption_module());
	print_test_return_status(test_aes128_block_cipher_mode_encryption_full());

	return CODE_SUCCESS;
}
