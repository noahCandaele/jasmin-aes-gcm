#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/utils.h"

extern void get_block_exp(uint8_t* strin, uint8_t* strout, int len, int blockID);

int test1() {
	printf("######## Test 1 get block 0, size 1 block ########\n");

	uint8_t arr_in[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
	uint8_t arr_out[NB_BYTES_128_BITS];

	get_block_exp(arr_in, arr_out, NB_BYTES_128_BITS, 0);

	printf("Expected output: "); print_uint8_array_as_hex(arr_in, NB_BYTES_128_BITS, true);
	printf("Actual output:   "); print_uint8_array_as_hex(arr_out, NB_BYTES_128_BITS, true);

	if(!compare_uint8_arrays(arr_in, arr_out, NB_BYTES_128_BITS)) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test2() {
	printf("######## Test 2 get block 0, size 1 block ########\n");

	char str[] = "ABCDEFGHIJKLMNOP";
	size_t size = strlen(str);
	printf("size %zu\n", size);
	uint8_t arr_in[size];
	convert_ascii_string_to_uint8_array(str, arr_in, size);
	uint8_t arr_out[NB_BYTES_128_BITS];

	get_block_exp(arr_in, arr_out, size, 0);

	printf("Expected output: ABCDEFGHIJKLMNOP\n");
	printf("Actual output:   "); print_uint8_array_as_ascii(arr_out, NB_BYTES_128_BITS, false);
	
	char str_out[NB_BYTES_128_BITS];
	convert_uint8_array_to_ascii_string(arr_out, NB_BYTES_128_BITS, str_out);
	if(strcmp(str_out, "ABCDEFGHIJKLMNOP") != 0) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test3() {
	printf("######## Test 3 get block 0, size 2 blocks ########\n");
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXZ1234567";
	size_t size = strlen(str);
	printf("size %zu\n", size);
	uint8_t arr_in[size];
	convert_ascii_string_to_uint8_array(str, arr_in, size);
	uint8_t arr_out[NB_BYTES_128_BITS];

	get_block_exp(arr_in, arr_out, size, 0);

	printf("Expected output: ABCDEFGHIJKLMNOP\n");
	printf("Actual output:   "); print_uint8_array_as_ascii(arr_out, NB_BYTES_128_BITS, false);
	
	char str_out[NB_BYTES_128_BITS];
	convert_uint8_array_to_ascii_string(arr_out, NB_BYTES_128_BITS, str_out);
	if(strcmp(str_out, "ABCDEFGHIJKLMNOP") != 0) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test4() {
	printf("######## Test 4 get block 1, size 2 blocks ########\n");
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXZ1234567";
	size_t size = strlen(str);
	printf("size %zu\n", size);
	uint8_t arr_in[size];
	convert_ascii_string_to_uint8_array(str, arr_in, size);
	uint8_t arr_out[NB_BYTES_128_BITS];

	get_block_exp(arr_in, arr_out, size, 1);

	printf("Expected output: QRSTUVWXZ1234567\n");
	printf("Actual output:   "); print_uint8_array_as_ascii(arr_out, NB_BYTES_128_BITS, false);
	
	char str_out[NB_BYTES_128_BITS];
	convert_uint8_array_to_ascii_string(arr_out, NB_BYTES_128_BITS, str_out);
	if(strcmp(str_out, "QRSTUVWXZ1234567") != 0) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test5() {
	printf("######## Test 5 get block 0, size between 1 and 2 blocks ########\n");
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXZ12";
	size_t size = strlen(str);
	printf("size %zu\n", size);
	uint8_t arr_in[size];
	convert_ascii_string_to_uint8_array(str, arr_in, size);
	uint8_t arr_out[NB_BYTES_128_BITS];

	get_block_exp(arr_in, arr_out, size, 0);

	printf("Expected output: ABCDEFGHIJKLMNOP\n");
	printf("Actual output:   "); print_uint8_array_as_ascii(arr_out, NB_BYTES_128_BITS, false);
	
	char str_out[NB_BYTES_128_BITS];
	convert_uint8_array_to_ascii_string(arr_out, NB_BYTES_128_BITS, str_out);
	if(strcmp(str_out, "ABCDEFGHIJKLMNOP") != 0) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test6() {
	printf("######## Test 6 get block 1, size between 1 and 2 blocks ########\n");
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXZ12";
	size_t size = strlen(str);
	printf("size %zu\n", size);
	uint8_t arr_in[size];
	convert_ascii_string_to_uint8_array(str, arr_in, size);
	char expected_str[] = "QRSTUVWXZ12";
	size_t len_expected = strlen(expected_str);
	uint8_t arr_out[len_expected];

	get_block_exp(arr_in, arr_out, size, 1);

	printf("Expected output: QRSTUVWXZ12\n");
	printf("Actual output:   "); print_uint8_array_as_ascii(arr_out, len_expected, false);
	
	char str_out[len_expected];
	convert_uint8_array_to_ascii_string(arr_out, len_expected, str_out);
	if(strcmp(str_out, expected_str) != 0) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int main()
{
	print_test_return_status(test1());
	print_test_return_status(test2());
	print_test_return_status(test3());
	print_test_return_status(test4());
	print_test_return_status(test5());
	print_test_return_status(test6());

	return 0;
}