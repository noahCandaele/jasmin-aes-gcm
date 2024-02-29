#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "utils.h"

extern void jasmin_u32(uint8_t* byte_array);
extern void jasmin_u64(uint8_t* byte_array);
extern void jasmin_u128(uint8_t* byte_array);

extern uint32_t jasmin_in_u32(uint8_t* byte_array);
extern void jasmin_in_out_u32(uint8_t* byte_array_in, uint8_t* byte_array_out);

extern void srink(uint8_t* ptr_byte_array, uint32_t* shrinked, uint32_t* shrinked2);


extern uint8_t int8_sens(uint8_t* byte_array, uint64_t index);

int test_cpu_compatibility(void) {
	printf("######## Checking CPU compatibility ########\n");
	return check_cpu_compatibility();
}

int test_print_uint8_array_as_binary() {
	printf("######## Test print uint8 array as binary ########\n");
	uint8_t arr[] = {0x01, 0x02, 0x03, 0x04};
    size_t size = sizeof(arr) / sizeof(arr[0]);
	printf("Expected output: 00000100 00000011 00000010 00000001\n");
    printf("Actual output:   "); print_uint8_array_as_binary(arr, size, true);

	uint8_t arr2[] = {0x01, 0x02, 0x03, 0x04};
    size_t size2 = sizeof(arr2) / sizeof(arr2[0]);
	printf("Expected output: 00000100000000110000001000000001\n");
    printf("Actual output:   "); print_uint8_array_as_binary(arr2, size2, false);

	return CODE_INFO;
}

int test_print_uint8_array_as_hex() {
	printf("######## Test print uint8 array as hexadecimal ########\n");
	uint8_t arr[] = {0x41, 0x42, 0x43, 0x44};
    size_t size = sizeof(arr) / sizeof(arr[0]);
	printf("Expected output: 44 43 42 41\n");
    printf("Actual output:   "); print_uint8_array_as_hex(arr, size, true);

	uint8_t arr2[] = {0xaa, 0xbb, 0xcc, 0xdd};
    size_t size2 = sizeof(arr2) / sizeof(arr2[0]);
	printf("Expected output: ddccbbaa\n");
    printf("Actual output:   "); print_uint8_array_as_hex(arr2, size2, false);

	return CODE_INFO;
}

int test_print_uint8_array_as_ascii() {
	printf("######## Test print uint8 array as ASCII ########\n");
	uint8_t arr[] = {68, 67, 66, 65};
    size_t size = sizeof(arr) / sizeof(arr[0]);

	printf("Expected output: ABCD\n");
    printf("Actual output:   "); print_uint8_array_as_ascii(arr, size, false);

	return CODE_INFO;
}

int test_print_uint8_array_as_hex_in_order() {
	printf("######## Test print uint8 array as hexadecimal in order ########\n");
	uint8_t arr[] = {0xaa, 0xbb, 0xcc, 0xdd};
    size_t size = sizeof(arr) / sizeof(arr[0]);
	printf("Expected output: aa bb cc dd\n");
    printf("Actual output:   "); print_uint8_array_as_hex_in_order(arr, size, true);

	return CODE_INFO;
}

int test_print_uint8_array_as_ascii_in_order() {
	printf("######## Test print uint8 array as ASCII in order ########\n");
	uint8_t arr[] = {97, 98, 99, 100};
    size_t size = sizeof(arr) / sizeof(arr[0]);

	printf("Expected output: abcd\n");
    printf("Actual output:   "); print_uint8_array_as_ascii_in_order(arr, size, false);

	return CODE_INFO;
}

int test_nb_bytes_hex_string() {
	printf("######## Test nb bytes hex string ########\n");

	char hex_str[] = "abcd";
	if(nb_bytes_hex_string(hex_str) != 2) {
		printf("Test 1 failed");
		return CODE_FAILURE;
	}

	char hex_str2[] = "";
	if(nb_bytes_hex_string(hex_str2) != 0) {
		printf("Test 2 failed");
		return CODE_FAILURE;
	}

	char hex_str3[] = "abcde";
	if(nb_bytes_hex_string(hex_str3) != 3) {
		printf("Test 3 failed");
		return CODE_FAILURE;
	}

	char hex_str4[] = "0123456789abcdef";
	if(nb_bytes_hex_string(hex_str4) != 8) {
		printf("Test 4 failed");
		return CODE_FAILURE;
	}

	char hex_str5[] = "a";
	if(nb_bytes_hex_string(hex_str5) != 1) {
		printf("Test 5 failed");
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_compare_uint8_arrays() {
	printf("######## Test compare uint8 arrays ########\n");

	uint8_t arr1[] = {65, 66, 67, 68};
	uint8_t arr2[] = {65, 66, 67, 68};
	if (!compare_uint8_arrays(arr1, arr2, 4)) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_convert_hex_string_to_uint8_array() {
	printf("######## Test convert hex string to uint8 array ########\n");

	char hex_str[] = "12abcdef";
	size_t nb_bytes = nb_bytes_hex_string(hex_str);

	uint8_t arr[nb_bytes];
	convert_hex_string_to_uint8_array(hex_str, arr, nb_bytes);

	uint8_t expected_arr[] = {0xef, 0xcd, 0xab, 0x12};
	if (!compare_uint8_arrays(arr, expected_arr, nb_bytes)) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_convert_ascii_string_to_uint8_array() {
	printf("######## Test convert ascii string to uint8 array ########\n");
	char ascii_string[] = "ABCD";
	size_t size = strlen(ascii_string);

	uint8_t uint8_array[size];
	convert_ascii_string_to_uint8_array(ascii_string, uint8_array, size);

	uint8_t expected_arr[] = {68, 67, 66, 65};
	if (!compare_uint8_arrays(uint8_array, expected_arr, size)) {
		return CODE_FAILURE;
	}
	
	return CODE_SUCCESS;
}

int test_convert_uint8_array_to_ascii_string() {
	printf("######## Test convert uint8 array to ascii string ########\n");

	uint8_t uint8_array[] = {0x72, 0x75, 0x6F, 0x6A, 0x6E, 0x6F, 0x62};
	size_t size = sizeof(uint8_array) / sizeof(uint8_array[0]);

	char ascii_string[size];
	convert_uint8_array_to_ascii_string(uint8_array, size, ascii_string);

	// printf("str: %s\n", ascii_string);

	char expected_string[] = "bonjour";
	uint8_t expected_arr[size];
	convert_ascii_string_to_uint8_array(expected_string, expected_arr, size);
	if (!compare_uint8_arrays(uint8_array, expected_arr, size)) {
		return CODE_FAILURE;
	}
	
	return CODE_SUCCESS;
}

int test_convert_hex_string_to_uint8_array_in_order() {
	printf("######## Test convert hex string to uint8 array in order ########\n");

	char hex_str[] = "12abcdef";
	size_t nb_bytes = nb_bytes_hex_string(hex_str);

	uint8_t arr[nb_bytes];
	convert_hex_string_to_uint8_array_in_order(hex_str, arr, nb_bytes);


	uint8_t expected_arr[] = {0x12, 0xab, 0xcd, 0xef};
	printf("Array contents: "); print_uint8_array_as_hex_in_order(arr, nb_bytes, true);
	printf("Expected array contents: "); print_uint8_array_as_hex_in_order(expected_arr, nb_bytes, true);
	if (!compare_uint8_arrays(arr, expected_arr, nb_bytes)) {
		return CODE_FAILURE;
	}

	return CODE_SUCCESS;
}

int test_convert_ascii_string_to_uint8_array_in_order() {
	printf("######## Test convert ascii string to uint8 array in order ########\n");
	char ascii_string[] = "ABCD";
	size_t size = strlen(ascii_string);

	uint8_t uint8_array[size];
	convert_ascii_string_to_uint8_array_in_order(ascii_string, uint8_array, size);

	uint8_t expected_arr[] = {65, 66, 67, 68};
	if (!compare_uint8_arrays(uint8_array, expected_arr, size)) {
		return CODE_FAILURE;
	}
	
	return CODE_SUCCESS;
}

int test_convert_uint8_array_to_ascii_string_in_order() {
	printf("######## Test convert uint8 array to ascii string in order ########\n");

	uint8_t uint8_array[] = {0x62, 0x6F, 0x6E, 0x6A, 0x6F, 0x75, 0x72};
	size_t size = sizeof(uint8_array) / sizeof(uint8_array[0]);

	char ascii_string[size];
	convert_uint8_array_to_ascii_string_in_order(uint8_array, size, ascii_string);

	printf("str: %s\n", ascii_string);

	char expected_string[] = "bonjour";
	uint8_t expected_arr[size];
	convert_ascii_string_to_uint8_array_in_order(expected_string, expected_arr, size);
	if (!compare_uint8_arrays(uint8_array, expected_arr, size)) {
		return CODE_FAILURE;
	}
	
	return CODE_SUCCESS;
}

int test_jasmin_u32() {
	printf("######## Test jasmin u32 ########\n");

	uint8_t byte_array[NB_BYTES_32_BITS];
	jasmin_u32(byte_array);
	printf("Original value: 0x01234567\n");
	printf("Array contents: ");
	for(int i = 0; i < NB_BYTES_32_BITS; i++) {
		printf("%02x ", (unsigned char)byte_array[i]);
	}
	printf("\n");

	return CODE_INFO;
}

int test_jasmin_u64() {
	printf("######## Test jasmin u64 ########\n");

	uint8_t byte_array[NB_BYTES_64_BITS];
	jasmin_u64(byte_array);

	printf("Original value: 0x0123456789abcdef\n");
	printf("Array contents: ");
	for(int i = 0; i < NB_BYTES_64_BITS; i++) {
		printf("%02x ", (unsigned char)byte_array[i]);
	}
	printf("\n");

	return CODE_INFO;
}

int test_jasmin_u128() {
	printf("######## Test jasmin u128 ########\n");

	uint8_t byte_array[NB_BYTES_128_BITS];
	jasmin_u128(byte_array);

	printf("Original value: 0x01020304050607080123456789abcdef\n");
	printf("Array contents: ");
	for(int i = 0; i < NB_BYTES_128_BITS; i++) {
		printf("%02x ", (unsigned char)byte_array[i]);
	}
	printf("\n");

	return CODE_INFO;
}

int test_jasmin_in_u32() {
	printf("######## Test jasmin in u32 ########\n");

	uint8_t byte_array[] = {0x01, 0x02, 0x03, 0x04};
	uint32_t res = jasmin_in_u32(byte_array);
	printf("Array contents: ");
	for(int i = 0; i < NB_BYTES_32_BITS; i++) {
		printf("%02x ", (unsigned char)byte_array[i]);
	}
	printf("\n");
	printf("returned value (uint32_t): %"PRIu32"\n", res);
	printf("returned value (hex): 0x%" PRIx32 "\n", res);

	return CODE_INFO;
}

int test_jasmin_in_out_u32() {
	printf("######## Test jasmin in out u32 ########\n");

	uint8_t byte_array_in[] = {0x01, 0x02, 0x03, 0x04};
	uint8_t byte_array_out[4];
	jasmin_in_out_u32(byte_array_in, byte_array_out);
	printf("Array contents in: ");
	for(int i = 0; i < NB_BYTES_32_BITS; i++) {
		printf("%02x ", (unsigned char)byte_array_in[i]);
	}
	printf("\n");
	printf("Array contents out: ");
	for(int i = 0; i < NB_BYTES_32_BITS; i++) {
		printf("%02x ", (unsigned char)byte_array_out[i]);
	}
	printf("\n");

	return CODE_INFO;
}

int test_int8_sens() {
	printf("######## Test int8_sens ########\n");

	uint64_t index = 1;
	uint8_t byte_array[NB_BYTES_128_BITS] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
											0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	uint8_t result = int8_sens(byte_array, index);

	printf("Expected value: %02x\n", byte_array[index]);
	printf("Actual value:   %02x\n", result);

	return CODE_INFO;
}

int test_shrink() {
	printf("######## Test shrink ########\n");
	uint8_t byte_array[NB_BYTES_128_BITS] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
											 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
	uint32_t extract;
	uint32_t extract2;
	srink(byte_array, &extract, &extract2);
	// printf("Expected value: %02x %02x\n", byte_array[0], byte_array[1]);
	// printf("Actual value:   %02x\n", extract);
	// check if extract is equal to extract2
	if(extract != extract2) {
		return CODE_FAILURE;
	} 

	return CODE_INFO;
}

int main()
{
	print_test_return_status(test_cpu_compatibility());

	print_test_return_status(test_print_uint8_array_as_binary());
	print_test_return_status(test_print_uint8_array_as_hex());
	print_test_return_status(test_print_uint8_array_as_ascii());

	print_test_return_status(test_print_uint8_array_as_hex_in_order());
	print_test_return_status(test_print_uint8_array_as_ascii_in_order());

	print_test_return_status(test_nb_bytes_hex_string());
	print_test_return_status(test_compare_uint8_arrays());

	print_test_return_status(test_convert_hex_string_to_uint8_array());
	print_test_return_status(test_convert_ascii_string_to_uint8_array());
	print_test_return_status(test_convert_uint8_array_to_ascii_string());

	print_test_return_status(test_convert_hex_string_to_uint8_array_in_order());
	print_test_return_status(test_convert_ascii_string_to_uint8_array_in_order());
	print_test_return_status(test_convert_uint8_array_to_ascii_string_in_order());

	print_test_return_status(test_jasmin_u32());
	print_test_return_status(test_jasmin_u64());
	print_test_return_status(test_jasmin_u128());

	print_test_return_status(test_jasmin_in_u32());
	print_test_return_status(test_jasmin_in_out_u32());

	print_test_return_status(test_int8_sens());
	print_test_return_status(test_shrink());

	return CODE_SUCCESS;
}