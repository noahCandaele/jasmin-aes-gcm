#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <smmintrin.h>
#include <stdbool.h>

#include "../utils/utils.h"

extern void ghash_jazz(uint8_t* a, uint8_t* b, uint8_t* out_res);
extern void ghash_xor_jazz(uint8_t* ghash_prev, uint8_t* data,  uint8_t* h, uint8_t* out_res);

int test_ghash_generic(char* data1_str, char* data2_str, char* ghash_expected_str) {
	uint8_t data1[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array(data1_str, data1, NB_BYTES_128_BITS);
	uint8_t data2[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array(data2_str, data2, NB_BYTES_128_BITS);

	printf("Input 1         (hex): "); print_uint8_array_as_hex(data1, NB_BYTES_128_BITS, false);
	printf("Input 2         (hex): "); print_uint8_array_as_hex(data2, NB_BYTES_128_BITS, false);

	uint8_t ghash_expected[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array(ghash_expected_str, ghash_expected, NB_BYTES_128_BITS);

	uint8_t ghash[NB_BYTES_128_BITS]; ghash_jazz(data1, data2, ghash);

	printf("Expected output (hex): "); print_uint8_array_as_hex(ghash_expected, NB_BYTES_128_BITS, false);
	printf("Actual output   (hex): "); print_uint8_array_as_hex(ghash, NB_BYTES_128_BITS, false);

	if(!compare_uint8_arrays(ghash, ghash_expected, NB_BYTES_128_BITS)) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_ghash_xor_generic(char* ghash_prev_str, char* data_str, char* h_str, char* ghash_expected_str) {
	uint8_t ghash_prev[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array(ghash_prev_str, ghash_prev, NB_BYTES_128_BITS);
	uint8_t data[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array(data_str, data, NB_BYTES_128_BITS);
	uint8_t h[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array(h_str, h, NB_BYTES_128_BITS);

	printf("Input 1         (hex): "); print_uint8_array_as_hex(ghash_prev, NB_BYTES_128_BITS, false);
	printf("Input 2         (hex): "); print_uint8_array_as_hex(data, NB_BYTES_128_BITS, false);
	printf("Input 3         (hex): "); print_uint8_array_as_hex(h, NB_BYTES_128_BITS, false);

	uint8_t ghash_expected[NB_BYTES_128_BITS]; convert_hex_string_to_uint8_array(ghash_expected_str, ghash_expected, NB_BYTES_128_BITS);

	uint8_t ghash[NB_BYTES_128_BITS]; ghash_xor_jazz(ghash_prev, data, h, ghash);

	printf("Expected output (hex): "); print_uint8_array_as_hex(ghash_expected, NB_BYTES_128_BITS, false);
	printf("Actual output   (hex): "); print_uint8_array_as_hex(ghash, NB_BYTES_128_BITS, false);

	if(!compare_uint8_arrays(ghash, ghash_expected, NB_BYTES_128_BITS)) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_ghash_nist2() {
	printf("######## ghash nist test case 2 ########\n");
	return test_ghash_generic("0388dace60b6a392f328c2b971b2fe78", "66e94bd4ef8a2c3b884cfa59ca342b2e", "5e2ec746917062882c85b0685353deb7");
}
int test_ghash_nist3() {
	printf("######## ghash nist test case 3 ########\n");
	return test_ghash_generic("42831ec2217774244b7221b784d0d49c", "b83b533708bf535d0aa6e52980d53b78", "59ed3f2bb1a0aaa07c9f56c6a504647b");
}
int test_ghash_nist4() {
	printf("######## ghash nist test case 4 ########\n");
	return test_ghash_generic("feedfacedeadbeeffeedfacedeadbeef", "b83b533708bf535d0aa6e52980d53b78", "ed56aaf8a72d67049fdb9228edba1322");
}
int test_ghash_nist8() {
	printf("######## ghash nist test case 8 ########\n");
	return test_ghash_generic("98e7247c07f0fe411c267e4384b0f600", "aae06992acbf52a3e8f4a96ec9300bd7", "90e87315fb7d4e1b4092ec0cbfda5d7d");
}
int test_ghash_nist9() {
	printf("######## ghash nist test case 9 ########\n");
	return test_ghash_generic("3980ca0b3c00e841eb06fac4872a2757", "466923ec9ae682214f2c082badb39249", "dddca3f91c17821ffac4a6d0fed176f7");
}
int test_ghash_nist10() {
	printf("######## ghash nist test case 10 ########\n");
	return test_ghash_generic("feedfacedeadbeeffeedfacedeadbeef", "466923ec9ae682214f2c082badb39249", "f3bf7ba3e305aeb05ed0d2e4fe076666");
}
int test_ghash_nist14() {
	printf("######## ghash nist test case 14 ########\n");
	return test_ghash_generic("cea7403d4d606b6e074ec5d3baf39d18", "dc95c078a2408989ad48a21492842087", "fd6ab7586e556dba06d69cfe6223b262");
}
int test_ghash_nist15() {
	printf("######## ghash nist test case 15 ########\n");
	return test_ghash_generic("522dc1f099567d07f47f37a32a84427d", "acbef20579b4b8ebce889bac8732dad7", "fcbefb78635d598eddaf982310670f35");
}
int test_ghash_nist16() {
	printf("######## ghash nist test case 16 ########\n");
	return test_ghash_generic("feedfacedeadbeeffeedfacedeadbeef", "acbef20579b4b8ebce889bac8732dad7", "5165d242c2592c0a6375e2622cf925d2");
}

int test_ghash_xor2() {
	printf("######## ghash xor nist test case 2 ########\n");
	return test_ghash_xor_generic("00000000000000000000000000000000", "0388dace60b6a392f328c2b971b2fe78", "66e94bd4ef8a2c3b884cfa59ca342b2e", "5e2ec746917062882c85b0685353deb7");
}
int test_ghash_xor3() {
	printf("######## ghash xor nist test case 3 ########\n");
	return test_ghash_xor_generic("59ed3f2bb1a0aaa07c9f56c6a504647b", "e3aa212f2c02a4e035c17e2329aca12e", "b83b533708bf535d0aa6e52980d53b78", "b714c9048389afd9f9bc5c1d4378e052");
}

int main()
{
	print_test_return_status(test_ghash_nist2());
	print_test_return_status(test_ghash_nist3());
	print_test_return_status(test_ghash_nist4());
	print_test_return_status(test_ghash_nist8());
	print_test_return_status(test_ghash_nist9());
	print_test_return_status(test_ghash_nist10());
	print_test_return_status(test_ghash_nist14());
	print_test_return_status(test_ghash_nist15());
	print_test_return_status(test_ghash_nist16());

	print_test_return_status(test_ghash_xor2());
	print_test_return_status(test_ghash_xor3());

	return CODE_SUCCESS;
}
