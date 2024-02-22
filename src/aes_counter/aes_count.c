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

int main() {
	// K feffe9928665731c6d6a8f9467308308
	// P d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255
	// IV cafebabefacedbaddecaf88800000001
	// 	C 42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985

// -------- Run --------
// Expected ciphertext: 42 83 1e c2 21 77 74 24 4b 72 21 b7 84 d0 d4 9c e3 aa 21 2f 2c 02 a4 e0 35 c1 7e 23 29 ac a1 2e 21 d5 14 b2 54 66 93 1c 7d 8f 6a 5a ac 84 aa 05 1b a3 0b 39 6a 0a ac 97 3d 58 e0 91 47 3f 59 85
// Actual ciphertext:   cf 4c 79 f8 07 ed ba 9e 10 f2 9b ff b0 0b 19 e5 5c 20 33 e9 2d ef d7 b0 df 44 76 00 0a 38 b7 bc 99 30 6f d8 34 9b 7b 8e cc ed 0a bd d8 a2 81 eb da 5a cd 4d ce b7 65 cb 57 a7 b9 c7 6d 20 af e6

// -------- Run --------
// Expected ciphertext: 42 83 1e c2 21 77 74 24 4b 72 21 b7 84 d0 d4 9c e3 aa 21 2f 2c 02 a4 e0 35 c1 7e 23 29 ac a1 2e 21 d5 14 b2 54 66 93 1c 7d 8f 6a 5a ac 84 aa 05 1b a3 0b 39 6a 0a ac 97 3d 58 e0 91 47 3f 59 85
// Actual ciphertext:   f1 76 d2 89 12 8a 38 b4 4a e8 03 1d 59 31 d0 cc 03 ab ca 1e b4 c7 85 07 cd 6e 34 a4 1b 35 be bc 85 cc 56 3b 23 07 94 f4 dc b2 80 3f 48 e4 03 0e 54 80 17 5d da 51 40 2a 6d c7 47 d0 96 0f f6 48

	char plain_hex[] = "00000000000000000000000000000000";
	size_t plain_length = nb_bytes_hex_string(plain_hex);
	char key_hex[] = "00000000000000000000000000000000";
	char ciphertext_expected_hex[] = "0388dace60b6a392f328c2b971b2fe78";
	char iv_hex[] = "00000000000000000000000000000001";

	uint8_t plain[plain_length];
	convert_hex_string_to_uint8_array(plain_hex, plain, plain_length);
	uint8_t key[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array(key_hex, key, NB_BYTES_128_BITS);
	uint8_t ciphertext_expected[plain_length];
	convert_hex_string_to_uint8_array(ciphertext_expected_hex, ciphertext_expected, plain_length);
	uint8_t iv[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array(iv_hex, iv, NB_BYTES_128_BITS);

	uint8_t ciphertext[plain_length];
	aes_count_iv(plain, ciphertext, plain_length, key, iv);

	printf("Expected ciphertext: "); print_uint8_array_as_hex(ciphertext_expected, plain_length, true);
	printf("Actual ciphertext:   "); print_uint8_array_as_hex(ciphertext, plain_length, true);




	// uint8_t key[NB_BYTES_128_BITS];
	// uint8_t iv[NB_BYTES_128_BITS];
	// convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array("f34481ec3cc627bacd5dc3fb08f273e6", iv, NB_BYTES_128_BITS);

	// // print key 
	// // for (size_t i = 0; i < NB_BYTES_128_BITS; ++i) {
	// // 	printf("%02x", (unsigned char)iv[i]);
	// // }
	// // printf("\n");
	// // convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	// // convert_hex_string_to_uint8_array("479be376295fc8547276c4c9a76d2822", cipher, NB_BYTES_128_BITS);

	// // convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// // convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", iv, NB_BYTES_128_BITS);
	// // convert_hex_string_to_uint8_array("00000000000000000000000000000000", iv, NB_BYTES_128_BITS);

	// char strin_no[] = "Two One Nine Two";
	// char* strin = (char*)malloc(sizeof(char) * strlen(strin_no));
	// strcpy(strin, strin_no);
	// // malloc strout
	// char* strout = (char*)malloc(sizeof(char) * 16 * (strlen(strin)/16 + 1));
	// // verify if malloc worked
	// if (strout == NULL) {
	// 	printf("Malloc failed\n");
	// 	return 1;
	// }
	// // fet N id block
	// long size = strlen(strin);
	// printf("Input string size: %ld\n", size);
	// aes_count_iv(strin, strout, size, key, iv);
	// printf("Output string: %s\n", strout);
	// printf("Output string size: %ld\n", strlen(strout));
	// // print strout as hex
	// // printf("Returned iv:\n");
	// // for (size_t i = 0; i < strlen(strout); ++i) {
	// // 	printf("%02x", (unsigned char)strout[i]);
	// // }
	// // printf("\n");

	// // for (int i = strlen(strout)-1; i >= 0; i--)
	// // {
	// // 	printf("%02x", (unsigned char)strout[i]);
	// // }
	// // printf("\n");

	// // printf("Initial iv:\nf34481ec3cc627bacd5dc3fb08f273e6\n");

	return 0;
}
