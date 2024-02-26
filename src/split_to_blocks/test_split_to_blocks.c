#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/utils.h"

// Convention: read the blocks in order, or in reverse?
bool CONVENTION_IN_ORDER = true;

extern void get_block_jazz(uint8_t* data, uint8_t* out_block, int data_length, int block_id);

int test_generic(char* data_str, int block_id, char* block_str_expected) {
	// Data
	printf("Data: %s\n", data_str);
	size_t data_length = strlen(data_str);
	printf("Data length: %zu\n", data_length);
	uint8_t data[data_length];
	if (CONVENTION_IN_ORDER) {
		convert_ascii_string_to_uint8_array_in_order(data_str, data, data_length);
	} else {
		convert_ascii_string_to_uint8_array(data_str, data, data_length);
	}

	// Expected block
	printf("Get block: %d\n", block_id);
	size_t block_length_expected = strlen(block_str_expected);
	printf("Block length expected: %zu\n", block_length_expected);
	uint8_t block_expected[block_length_expected];
	if (CONVENTION_IN_ORDER) {
		convert_ascii_string_to_uint8_array_in_order(block_str_expected, block_expected, block_length_expected);
	} else {
		convert_ascii_string_to_uint8_array(block_str_expected, block_expected, block_length_expected);
	}

	// Get block
	uint8_t block[block_length_expected];
	get_block_jazz(data, block, data_length, block_id);
	
	// Print output
	printf("Expected output: %s\n", block_str_expected);
	printf("Actual output:   ");
	if (CONVENTION_IN_ORDER) {
		print_uint8_array_as_ascii_in_order(block, block_length_expected, false);
	} else {
		print_uint8_array_as_ascii(block, block_length_expected, false);
	}
	
	// Compare
	if(!compare_uint8_arrays(block, block_expected, block_length_expected)) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_len0x16_get0() {
	printf("######## Test: length 0*16 bytes, get block 0 ########\n");
	return test_generic("", 0, "");
}

int test_len0x16p9_get0() {
	printf("######## Test: length 0*16+9 bytes, get block 0 ########\n");
	return test_generic("abcdefghi", 0, "abcdefghi");
}

int test_len1x16_get0() {
	printf("######## Test: length 1*16 bytes, get block 0 ########\n");
	return test_generic("abcdefghijklmnop", 0, "abcdefghijklmnop");
}

int test_len1x16p4_get0() {
	printf("######## Test: length 1*16+4 bytes, get block 0 ########\n");
	return test_generic("abcdefghijklmnopqrst", 0, "abcdefghijklmnop");
}
int test_len1x16p4_get1() {
	printf("######## Test: length 1*16+4 bytes, get block 1 ########\n");
	return test_generic("abcdefghijklmnopqrst", 1, "qrst");
}


int test_len3x16_get0() {
	printf("######## Test: length 3*16 bytes, get block 0 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKL", 0, "abcdefghijklmnop");
}
int test_len3x16_get1() {
	printf("######## Test: length 3*16 bytes, get block 1 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKL", 1, "qrstuvwxyz123456");
}
int test_len3x16_get2() {
	printf("######## Test: length 3*16 bytes, get block 2 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKL", 2, "7890ABCDEFGHIJKL");
}
int test_len3x16_get3() {
	printf("######## Test: length 3*16 bytes, get block 3 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKL", 3, "");
}

int test_len3x16p7_get0() {
	printf("######## Test: length 3*16+7 bytes, get block 0 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRS", 0, "abcdefghijklmnop");
}
int test_len3x16p7_get1() {
	printf("######## Test: length 3*16+7 bytes, get block 1 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRS", 1, "qrstuvwxyz123456");
}
int test_len3x16p7_get2() {
	printf("######## Test: length 3*16+7 bytes, get block 2 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRS", 2, "7890ABCDEFGHIJKL");
}
int test_len3x16p7_get3() {
	printf("######## Test: length 3*16+7 bytes, get block 3 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRS", 3, "MNOPQRS");
}
int test_len3x16p7_get4() {
	printf("######## Test: length 3*16+7 bytes, get block 4 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRS", 4, "");
}

int test_len6x16_get4() {
	printf("######## Test: length 6*16 bytes, get block 4 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678", 4, "cdefghijklmnopqr");
}
int test_len6x16p2_get5() {
	printf("######## Test: length 6*16+2 bytes, get block 5 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890", 5, "stuvwxyz12345678");
}
int test_len6x16p2_get6() {
	printf("######## Test: length 6*16+2 bytes, get block 6 ########\n");
	return test_generic("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890", 6, "90");
}

int main()
{
	print_test_return_status(test_len0x16_get0());
	print_test_return_status(test_len0x16p9_get0());

	print_test_return_status(test_len1x16_get0());
	print_test_return_status(test_len1x16p4_get0());
	print_test_return_status(test_len1x16p4_get1());

	print_test_return_status(test_len3x16_get0());
	print_test_return_status(test_len3x16_get1());
	print_test_return_status(test_len3x16_get2());
	print_test_return_status(test_len3x16_get3());
	print_test_return_status(test_len3x16p7_get0());
	print_test_return_status(test_len3x16p7_get1());
	print_test_return_status(test_len3x16p7_get2());
	print_test_return_status(test_len3x16p7_get3());
	print_test_return_status(test_len3x16p7_get4());

	print_test_return_status(test_len6x16_get4());
	print_test_return_status(test_len6x16p2_get5());
	print_test_return_status(test_len6x16p2_get6());

	return 0;
}
