#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "utils.h"

extern void jasmin_u32(uint8_t* byte_array);
extern void jasmin_u64(uint8_t* byte_array);
extern void jasmin_u128(uint8_t* byte_array);

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

	char hex_str[] = "12abcdef";
	size_t nb_bytes = nb_bytes_hex_string(hex_str);

	uint8_t arr[nb_bytes];
	convert_hex_string_to_uint8_array(hex_str, arr, nb_bytes);

	uint8_t expected_arr[] = {0xef, 0xcd, 0xab, 0x12};
	if (!compare_uint8_arrays(arr, expected_arr, nb_bytes)) {
		return CODE_FAIL;
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
		return CODE_FAIL;
	}
	
	return CODE_SUCCESS;
}

int test_convert_uint8_array_to_ascii_string() {
	printf("######## Test convert uint8 array to ascii string ########\n");

	uint8_t uint8_array[] = {0x72, 0x75, 0x6F, 0x6A, 0x6E, 0x6F, 0x62};
	size_t size = sizeof(uint8_array) / sizeof(uint8_array[0]);

	char ascii_string[size];
	convert_uint8_array_to_ascii_string(uint8_array, size, ascii_string);

	printf("str: %s\n", ascii_string); //TODO

	// char expected_string[] = "bonjour";
	// if (!compare_uint8_arrays(uint8_array, expected_arr, size)) {
	// 	return CODE_FAIL;
	// }
	
	return CODE_INFO;
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

int main()
{
	print_test_return_status(test_cpu_compatibility());

	print_test_return_status(test_print_uint8_array_as_binary());
	print_test_return_status(test_print_uint8_array_as_hex());
	print_test_return_status(test_print_uint8_array_as_ascii());

	print_test_return_status(test_nb_bytes_hex_string());
	print_test_return_status(test_compare_uint8_arrays());

	print_test_return_status(test_convert_hex_string_to_uint8_array());
	print_test_return_status(test_convert_ascii_string_to_uint8_array());
	print_test_return_status(test_convert_uint8_array_to_ascii_string());

	print_test_return_status(test_jasmin_u32());
	print_test_return_status(test_jasmin_u64());
	print_test_return_status(test_jasmin_u128());

	return CODE_SUCCESS;
}