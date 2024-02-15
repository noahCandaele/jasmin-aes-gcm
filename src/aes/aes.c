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

#define RED_TEXT "\x1b[31m"
#define YELLOW_TEXT "\x1b[33m"
#define GREEN_TEXT "\x1b[32m"
#define RESET_COLOR "\x1b[0m"

#define RETURN_SUCCESS 0
#define RETURN_FAIL 1
#define RETURN_WARNING 2

#define BYTE_ARRAY_SIZE_128_BIT 16
#define BYTE_ARRAY_SIZE_192_BIT 24
#define BYTE_ARRAY_SIZE_256_BIT 32

extern __m128i aes_jazz(__m128i key, __m128i plain);
extern __m128i invaes_jazz(__m128i key, __m128i cipher);

static inline void native_cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
	asm volatile("cpuid"
				 : "=a" (*eax),
				   "=b" (*ebx),
				   "=c" (*ecx),
				   "=d" (*edx)
				 : "0" (*eax), "2" (*ecx));
}

static int check_cpuflags(void) {
	printf("Checking CPU compatibility\n");

	uint32_t eax = 1, ebx, ecx, edx;
	int arggl = 0;

	native_cpuid(&eax, &ebx, &ecx, &edx);

	if (!(ecx & (1 << 25))) {
		(void) fprintf(stderr, "Your CPU does not seem to have the AES instruction set\n");
		arggl += 1;
		return RETURN_WARNING;
	}

	if (!(ecx & (1 << 28))) {
		(void) fprintf(stderr, "Your CPU does not seem to have the AVX instruction set\n");
		arggl += 1;
		return RETURN_WARNING;
	}

	if (arggl) {
		(void) fprintf(stderr, "There is a high probability that the program triggers "
			"an \"Illegal instruction\" exception\n");
		return RETURN_WARNING;
	}

	return RETURN_SUCCESS;
}

void print_return_status(int return_code)
{
	if (return_code == RETURN_SUCCESS)
	{
		printf(GREEN_TEXT "Test successful\n" RESET_COLOR);
	}
	else if (return_code == RETURN_WARNING)
	{
		printf(YELLOW_TEXT "Test returned with warnings\n" RESET_COLOR);
	}
	else
	{
		printf(RED_TEXT "Test failed\n" RESET_COLOR);
	}
}

void string_hex_to_int8_array(char hex_string[], int8_t* int8_array, size_t array_size) {
	if (array_size != strlen(hex_string) / 2) {
		printf(YELLOW_TEXT "Warning: string_hex_to_int8_array: trying to convert hex string of %zu bytes to array of %zu bytes\n" RESET_COLOR, strlen(hex_string) / 2, array_size);
	}
	
	for (size_t i = 0; i < array_size; ++i) {
		char hex[2] = { hex_string[i * 2], hex_string[i * 2 + 1] };
		// cast to int8_t
		int8_array[i] = (int8_t)strtol(hex, NULL, 16);
	}
}

bool compare_int8_arrays(const int8_t* array1, const int8_t* array2, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (array1[i] != array2[i]) {
            return false;
        }
    }
    return true;
}

void print_int8_array(int8_t arr[], size_t size) {
	for (size_t i = 0; i < size; ++i) {
		printf("%02x", (unsigned char)arr[i]);

		// Add a space after every 4 characters
		if ((i + 1) % 2 == 0 && i + 1 < size) {
			printf(" ");
		}
	}
	printf("\n");
}

void print_ascii_representation(int8_t arr[], size_t size) {
	for (size_t i = 0; i < size; ++i) {
		printf("%c ", (char)arr[i]);

		// Add a space after every 4 characters
		if ((i + 1) % 2 == 0 && i + 1 < size) {
			printf(" ");
		}
	}
	printf("\n");
}

__m128i arr_to_u128(int8_t* arr) {
	return _mm_loadu_si128((__m128i *) arr);
}

void u128_to_arr(__m128i value, int8_t* arr) {
	_mm_storeu_si128((__m128i *)arr, value);
}

__m256i arr_to_u256(int8_t* arr) {
    __m128i* ptr = (__m128i*)arr;
    __m256i result = _mm256_set_m128i(_mm_loadu_si128(ptr + 1), _mm_loadu_si128(ptr));
    return result;
}

void aes(int8_t* key, int8_t* plain, int8_t* cipher_computed) {
	u128_to_arr(aes_jazz(arr_to_u128(key), arr_to_u128(plain)), cipher_computed);
}

void invaes(int8_t* key, int8_t* cipher, int8_t* plain_computed) {
	u128_to_arr(invaes_jazz(arr_to_u128(key), arr_to_u128(cipher)), plain_computed);
}

int test_aes128() {
	printf("Test AES-128\n");
	printf("\n");

	printf("Test 1\n");
	printf("\n");
	
	int8_t* key = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	int8_t* plain = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	int8_t* cipher = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	if (key == NULL || plain == NULL || cipher == NULL) {
		printf("test_aes128: allocation of key, plain or cipher failed");
		return RETURN_FAIL;
	}
	string_hex_to_int8_array("5468617473206D79204B756E67204675", key, BYTE_ARRAY_SIZE_128_BIT);
	string_hex_to_int8_array("54776F204F6E65204E696E652054776F", plain, BYTE_ARRAY_SIZE_128_BIT);
	string_hex_to_int8_array("29C3505F571420F6402299B31A02D73A", cipher, BYTE_ARRAY_SIZE_128_BIT);

	printf("key (hex)     : "); print_int8_array(key, BYTE_ARRAY_SIZE_128_BIT);
	printf("key (ascii)   : "); print_ascii_representation(key, BYTE_ARRAY_SIZE_128_BIT);
	printf("\n");
	printf("plain (hex)   : "); print_int8_array(plain, BYTE_ARRAY_SIZE_128_BIT);
	printf("plain (ascii) : "); print_ascii_representation(plain, BYTE_ARRAY_SIZE_128_BIT);
	printf("\n");
	printf("cipher (hex)  : "); print_int8_array(cipher, BYTE_ARRAY_SIZE_128_BIT);
	printf("cipher (ascii): "); print_ascii_representation(cipher, BYTE_ARRAY_SIZE_128_BIT);
	printf("\n");

	printf("Jasmin-generated encryption\n");
	int8_t* cipher_computed = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	if (cipher_computed == NULL) {
		printf("test_aes128: allocation of cipher_computed failed");
		return RETURN_FAIL;
	}
	aes(key, plain, cipher_computed);
	printf("computed cipher (hex)  : "); print_int8_array(cipher_computed, BYTE_ARRAY_SIZE_128_BIT);
	printf("computed cipher (ascii): "); print_ascii_representation(cipher_computed, BYTE_ARRAY_SIZE_128_BIT);
	printf("\n");

	printf("Jasmin-generated decryption\n");
	int8_t* plain_computed = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	if (plain_computed == NULL) {
		printf("test_aes128: allocation of plain_computed failed");
		return RETURN_FAIL;
	}
	invaes(key, cipher_computed, plain_computed);
	printf("computed plain (hex)  : "); print_int8_array(plain_computed, BYTE_ARRAY_SIZE_128_BIT);
	printf("computed plain (ascii): "); print_ascii_representation(plain_computed, BYTE_ARRAY_SIZE_128_BIT);
	printf("\n");

	// Compare expected values with computed values
	bool plain_success = compare_int8_arrays(plain, plain_computed, BYTE_ARRAY_SIZE_128_BIT);
	if(plain_success) {
		printf("Success: plain has been computed as expected\n");
	} else {
		printf("Failure: plain has not been computed as expected\n");
		return RETURN_FAIL;
	}
	bool cipher_success = compare_int8_arrays(cipher, cipher_computed, BYTE_ARRAY_SIZE_128_BIT);
	if(cipher_success) {
		printf("Success: cipher has been computed as expected\n");
	} else {
		printf("Failure: cipher has not been computed as expected\n");
		return RETURN_FAIL;
	}

	free(key); free(plain); free(cipher); free(cipher_computed); free(plain_computed);

	// #######################################################
	printf("\n");
	printf("Test 2\n");

	int8_t key2[]    = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	int8_t plain2[]  = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	int8_t cipher2[] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};

	int8_t* cipher_computed2 = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	if (cipher_computed2 == NULL) {
		printf("test_aes128: allocation of cipher_computed2 failed");
		return RETURN_FAIL;
	}
	aes(key2, plain2, cipher_computed2);

	int8_t* plain_computed2 = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	if (plain_computed2 == NULL) {
		printf("test_aes128: allocation of plain_computed2 failed");
		return RETURN_FAIL;
	}
	invaes(key2, cipher_computed2, plain_computed2);

	// Compare expected values with computed values
	bool plain_success2 = compare_int8_arrays(plain2, plain_computed2, BYTE_ARRAY_SIZE_128_BIT);
	if(plain_success2) {
		printf("Success: plain2 has been computed as expected\n");
	} else {
		printf("Failure: plain2 has not been computed as expected\n");
		return RETURN_FAIL;
	}
	bool cipher_success2 = compare_int8_arrays(cipher2, cipher_computed2, BYTE_ARRAY_SIZE_128_BIT);
	if(cipher_success2) {
		printf("Success: cipher2 has been computed as expected\n");
	} else {
		printf("Failure: cipher2 has not been computed as expected\n");
		return RETURN_FAIL;
	}

	return RETURN_SUCCESS;
}

int test_aes256() {
	printf("Test AES-256\n");
	printf("\n");
	
	int8_t* key = (int8_t*)malloc(BYTE_ARRAY_SIZE_256_BIT * sizeof(int8_t));
	int8_t* plain = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	int8_t* cipher = (int8_t*)malloc(BYTE_ARRAY_SIZE_128_BIT * sizeof(int8_t));
	if (key == NULL || plain == NULL || cipher == NULL) {
		printf("test_aes256: allocation of key, plain or cipher failed");
		return RETURN_FAIL;
	}
	string_hex_to_int8_array("c47b0294dbbbee0fec4757f22ffeee3587ca4730c3d33b691df38bab076bc558", key, BYTE_ARRAY_SIZE_256_BIT);
	string_hex_to_int8_array("00000000000000000000000000000000", plain, BYTE_ARRAY_SIZE_128_BIT);
	string_hex_to_int8_array("46f2fb342d6f0ab477476fc501242c5f", cipher, BYTE_ARRAY_SIZE_128_BIT);

	// TODO

	free(key); free(plain); free(cipher);

	return RETURN_FAIL;
}

int main() {
	print_return_status(check_cpuflags());

	printf("___________________________________\n");
	print_return_status(test_aes128());

	// printf("___________________________________\n");
	// print_return_status(test_aes256());

	return RETURN_SUCCESS;
}
