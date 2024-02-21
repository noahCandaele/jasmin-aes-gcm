#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "../utils/utils.h"

void aes_count(char* strin, char* strout, int len, int8_t* key);
void aes_count_iv(char* strin, char* strout, int len, int8_t* key, int8_t* iv);

int main() {
	uint8_t key[NB_BYTES_128_BITS];
	uint8_t iv[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("f34481ec3cc627bacd5dc3fb08f273e6", iv, NB_BYTES_128_BITS);

	// print key 
	// for (size_t i = 0; i < NB_BYTES_128_BITS; ++i) {
	// 	printf("%02x", (unsigned char)iv[i]);
	// }
	// printf("\n");
	// convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array("479be376295fc8547276c4c9a76d2822", cipher, NB_BYTES_128_BITS);

	// convert_hex_string_to_uint8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array("54776F204F6E65204E696E652054776F", iv, NB_BYTES_128_BITS);
	// convert_hex_string_to_uint8_array("00000000000000000000000000000000", iv, NB_BYTES_128_BITS);

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
	aes_count_iv(strin, strout, size, key, iv);
	printf("Output string: %s\n", strout);
	printf("Output string size: %ld\n", strlen(strout));
	// print strout as hex
	// printf("Returned iv:\n");
	// for (size_t i = 0; i < strlen(strout); ++i) {
	// 	printf("%02x", (unsigned char)strout[i]);
	// }
	// printf("\n");

	// for (int i = strlen(strout)-1; i >= 0; i--)
	// {
	// 	printf("%02x", (unsigned char)strout[i]);
	// }
	// printf("\n");

	// printf("Initial iv:\nf34481ec3cc627bacd5dc3fb08f273e6\n");

	return 0;
}
