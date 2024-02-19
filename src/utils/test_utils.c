#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "utils.h"

int test_print_uint8_array_as_binary() {
	printf("######## Test print uint8 array as binary ########\n");
	uint8_t arr[] = {0x04, 0x03, 0x02, 0x01};
    size_t size = sizeof(arr) / sizeof(arr[0]);

	printf("Expected output: 00000100 00000011 00000010 00000001\n");
    printf("Actual output:   "); print_uint8_array_as_binary(arr, size, true);

	return CODE_INFO;
}

int test_print_uint8_array_as_hex() {
	printf("######## Test print uint8 array as hexadecimal ########\n");
	uint8_t arr[] = {0x41, 0x42, 0x43, 0x44};
    size_t size = sizeof(arr) / sizeof(arr[0]);

	printf("Expected output: 4142 4344\n");
    printf("Actual output:   "); print_uint8_array_as_hex(arr, size, true);

	return CODE_INFO;
}

int test_print_uint8_array_as_ascii() {
	printf("######## Test print uint8 array as ASCII ########\n");
	uint8_t arr[] = {65, 66, 67, 68};
    size_t size = sizeof(arr) / sizeof(arr[0]);

	printf("Expected output: ABCD\n");
    printf("Actual output:   "); print_uint8_array_as_ascii(arr, size, false);

	return CODE_INFO;
}

int test_nb_bytes_hex_string() {
	printf("######## Test nb bytes hex string ########\n");

	char hex_str[] = "abcd";
	if(nb_bytes_hex_string(hex_str) != 2) {
		printf("Test 1 failed");
		return CODE_FAIL;
	}

	char hex_str2[] = "";
	if(nb_bytes_hex_string(hex_str2) != 0) {
		printf("Test 2 failed");
		return CODE_FAIL;
	}

	char hex_str3[] = "abcde";
	if(nb_bytes_hex_string(hex_str3) != 3) {
		printf("Test 3 failed");
		return CODE_FAIL;
	}

	char hex_str4[] = "0123456789abcdef";
	if(nb_bytes_hex_string(hex_str4) != 8) {
		printf("Test 4 failed");
		return CODE_FAIL;
	}

	char hex_str5[] = "a";
	if(nb_bytes_hex_string(hex_str5) != 1) {
		printf("Test 5 failed");
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test_compare_uint8_arrays() {
	printf("######## Test compare uint8 arrays ########\n");

	uint8_t arr1[] = {65, 66, 67, 68};
	uint8_t arr2[] = {65, 66, 67, 68};
	if (!compare_uint8_arrays(arr1, arr2, 4)) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int test_convert_hex_string_to_uint8_array() {
	printf("######## Test convert hex string to uint8 array ########\n");

	char hex_str[] = "12ab";
	size_t nb_bytes = nb_bytes_hex_string(hex_str);
	printf("nb_bytes: %zu\n", nb_bytes);

	uint8_t arr[nb_bytes];
	convert_hex_string_to_uint8_array(hex_str, arr, nb_bytes);

	uint8_t expected_arr[] = {18, 171}; // for hex "12ab": 2 bytes
	if (!compare_uint8_arrays(arr, expected_arr, nb_bytes)) {
		return CODE_FAIL;
	}

    printf("hex: "); print_uint8_array_as_hex(arr, nb_bytes, true);
    printf("bin: "); print_uint8_array_as_binary(arr, nb_bytes, true);

	return CODE_SUCCESS;
}

int test_convert_uint64_to_uint8_array() {
	printf("######## Test convert uint64 to uint8 array ########\n");
	
	uint64_t value = 0b0000100000000111000001100000010100000100000000110000001000000001;
	uint8_t arr[NB_BYTES_64_BITS];
	convert_uint64_to_uint8_array(value, arr);

	printf("hex: "); print_uint8_array_as_hex(arr, NB_BYTES_64_BITS, true);
	printf("bin: "); print_uint8_array_as_binary(arr, NB_BYTES_64_BITS, true);

	uint8_t expected_arr[] = {0b00001000, 0b00000111, 0b00000110, 0b00000101, 0b00000100, 0b00000011, 0b00000010, 0b00000001};
	if (!compare_uint8_arrays(arr, expected_arr, NB_BYTES_64_BITS)) {
		return CODE_FAIL;
	}

	return CODE_SUCCESS;
}

int main()
{
	print_test_return_status(test_print_uint8_array_as_binary());
	print_test_return_status(test_print_uint8_array_as_hex());
	print_test_return_status(test_print_uint8_array_as_ascii());

	print_test_return_status(test_nb_bytes_hex_string());
	print_test_return_status(test_compare_uint8_arrays());

	print_test_return_status(test_convert_hex_string_to_uint8_array());
	print_test_return_status(test_convert_uint64_to_uint8_array());

	return CODE_SUCCESS;
}