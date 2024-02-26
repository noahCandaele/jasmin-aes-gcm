#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <smmintrin.h>
#include <stdbool.h>

#include "../utils/utils.h"

extern void ghash_jazz(uint8_t* input, uint8_t* h, uint32_t i, uint8_t* out_res);
// extern void ghash_xor_jazz(uint8_t* ghash_prev, uint8_t* data,  uint8_t* h, uint32_t i, uint8_t* out_res);
// extern void tag_jazz(uint8_t* h, uint8_t* ghash, uint8_t* out_tag);

extern void reflecting_bit(uint8_t* in);

const bool CONVENTION_IN_ORDER = true;

// int test_ghash() {
	
// 	printf("######## Test ghash ########\n");

// 	uint8_t k[NB_BYTES_128_BITS];
// 	uint8_t p[NB_BYTES_128_BITS];
// 	uint8_t iv[NB_BYTES_128_BITS];
// 	uint8_t iv0[NB_BYTES_128_BITS];
// 	uint8_t c[NB_BYTES_128_BITS];
// 	uint8_t h[NB_BYTES_128_BITS];
// 	uint8_t len[NB_BYTES_128_BITS];
	
// 	convert_hex_string_to_uint8_array("00000000000000000000000000000000", k, NB_BYTES_128_BITS);
// 	convert_hex_string_to_uint8_array("00000000000000000000000000000000", p, NB_BYTES_128_BITS);
// 	convert_hex_string_to_uint8_array("00000000000000000000000000000001", iv, NB_BYTES_128_BITS);
// 	convert_hex_string_to_uint8_array("00000000000000000000000000000000", iv0, NB_BYTES_128_BITS);
// 	convert_hex_string_to_uint8_array("0388dace60b6a392f328c2b971b2fe78", c, NB_BYTES_128_BITS);
// 	convert_hex_string_to_uint8_array("66e94bd4ef8a2c3b884cfa59ca342b2e", h, NB_BYTES_128_BITS);
// 	convert_hex_string_to_uint8_array("00000000000000000000000000000080", len, NB_BYTES_128_BITS);

// 	uint8_t ghash1[NB_BYTES_128_BITS];
// 	ghash_jazz(c, h, 0, ghash1);
// 	print_uint8_array_as_hex(ghash1, NB_BYTES_128_BITS, false);

// 	uint8_t ghash2[NB_BYTES_128_BITS];
// 	ghash_xor_jazz(ghash1, len, h, 1, ghash2);
// 	print_uint8_array_as_hex(ghash2, NB_BYTES_128_BITS, false);

// 	uint8_t tag[NB_BYTES_128_BITS];
// 	tag_jazz(h, ghash2, tag);
// 	print_uint8_array_as_hex(tag, NB_BYTES_128_BITS, false);

// 	return CODE_INFO;
// }

int test_ghash_nist2() {
	
	printf("######## Test ghash NIST test case 2 ########\n");

	uint8_t c[NB_BYTES_128_BITS];
	uint8_t h[NB_BYTES_128_BITS];

	convert_hex_string_to_uint8_array("0388dace60b6a392f328c2b971b2fe78", c, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("66e94bd4ef8a2c3b884cfa59ca342b2e", h, NB_BYTES_128_BITS);

	uint8_t ghash[NB_BYTES_128_BITS];
	ghash_jazz(c, h, 0, ghash);
	print_uint8_array_as_hex(ghash, NB_BYTES_128_BITS, false);
	print_uint8_array_as_binary(ghash, NB_BYTES_128_BITS, true);

	return CODE_INFO;
}

int test_ghash_nist3() {
	printf("######## Test ghash NIST test case 3 ########\n");

	uint8_t c[NB_BYTES_128_BITS];
	uint8_t h[NB_BYTES_128_BITS];

	convert_hex_string_to_uint8_array_in_order("1ba30b396a0aac973d58e091473f5985", c, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order("b83b533708bf535d0aa6e52980d53b78", h, NB_BYTES_128_BITS);

	uint8_t ghash[NB_BYTES_128_BITS];
	ghash_jazz(c, h, 0, ghash);
	print_uint8_array_as_hex_in_order(ghash, NB_BYTES_128_BITS, false);

	return CODE_INFO;
}

int test_ghash_generic(char* data1_str, char* data2_str, char* ghash_expected_str, bool in_order) {
	uint8_t data1[NB_BYTES_128_BITS];
	uint8_t data2[NB_BYTES_128_BITS];
	if (in_order) convert_hex_string_to_uint8_array_in_order(data1_str, data1, NB_BYTES_128_BITS);
	else convert_hex_string_to_uint8_array(data1_str, data1, NB_BYTES_128_BITS);
	if (in_order) convert_hex_string_to_uint8_array_in_order(data2_str, data2, NB_BYTES_128_BITS);
	else convert_hex_string_to_uint8_array(data2_str, data2, NB_BYTES_128_BITS);

	printf("Input 1         (hex): ");
	if (in_order) print_uint8_array_as_hex_in_order(data1, NB_BYTES_128_BITS, true);
	else print_uint8_array_as_hex(data1, NB_BYTES_128_BITS, true);
	printf("Input 2         (hex): ");
	if (in_order) print_uint8_array_as_hex_in_order(data2, NB_BYTES_128_BITS, true);
	else print_uint8_array_as_hex(data2, NB_BYTES_128_BITS, true);

	uint8_t ghash_expected[NB_BYTES_128_BITS];
	if (in_order) convert_hex_string_to_uint8_array_in_order(ghash_expected_str, ghash_expected, NB_BYTES_128_BITS);
	else convert_hex_string_to_uint8_array(ghash_expected_str, ghash_expected, NB_BYTES_128_BITS);

	uint8_t ghash[NB_BYTES_128_BITS];
	ghash_jazz(data1, data2, 0, ghash);



	printf("Expected output (hex): ");
	if (in_order) print_uint8_array_as_hex_in_order(ghash_expected, NB_BYTES_128_BITS, true);
	else print_uint8_array_as_hex(ghash_expected, NB_BYTES_128_BITS, true);
	printf("Actual output   (hex): ");
	if (in_order) print_uint8_array_as_hex_in_order(ghash, NB_BYTES_128_BITS, true);
	else print_uint8_array_as_hex(ghash, NB_BYTES_128_BITS, true);

	// printf("Expected output (bin): ");
	// print_uint8_array_as_binary(ghash_expected, NB_BYTES_128_BITS, true);
	// printf("Actual output   (bin): ");
	// print_uint8_array_as_binary(ghash, NB_BYTES_128_BITS, true);

	if(!compare_uint8_arrays(ghash, ghash_expected, NB_BYTES_128_BITS)) {
		// printf("Error: expected output and actual output are different.\n");
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}


int test_ghash_nist2_sure() {
	printf("######## Test ghash NIST test case 2 absolutely sure ########\n");

	return test_ghash_generic("0388dace60b6a392f328c2b971b2fe78", "66e94bd4ef8a2c3b884cfa59ca342b2e", "5e2ec746917062882c85b0685353deb7", false);
}
int test_ghash_nist2_sure_reverse() {
	printf("######## Test ghash NIST test case 2 absolutely sure REVERSE ########\n");
	return test_ghash_generic("0388dace60b6a392f328c2b971b2fe78", "66e94bd4ef8a2c3b884cfa59ca342b2e", "5e2ec746917062882c85b0685353deb7", true);
}

int test_reflect() {
	printf("######## Test reflect ########\n");

	uint8_t data[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array("0388dace60b6a392f328c2b971b2fe78", data, NB_BYTES_128_BITS);
	printf("Input  binary: ");
	print_uint8_array_as_binary(data, NB_BYTES_128_BITS, false);

	reflecting_bit(data);
	printf("Output binary: ");
	print_uint8_array_as_binary(data, NB_BYTES_128_BITS, false);

	// 00011110011111110100110110001110100111010100001100010100110011110100100111000101011011010000011001110011010110110001000111000000

	return CODE_INFO;
}

int main()
{
	// print_test_return_status(test_ghash());
	// print_test_return_status(test_ghash_nist3());

	print_test_return_status(test_ghash_nist2_sure());
	// print_test_return_status(test_ghash_nist2_sure_reverse());
	// print_test_return_status(test_ghash_nist2());
	// print_test_return_status(test_reflect());

	return 0;
}

// Expected output: 	5e2ec746917062882c85b0685353deb7
// no inversed and v1: 	519fa38ac731568e9c1eb21731167f1c
