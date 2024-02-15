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

#define ARRAY_SIZE_128_BIT 16
#define ARRAY_SIZE_192_BIT 24
#define ARRAY_SIZE_256_BIT 32

extern __m128i aes_jazz(__m256i key, __m128i plain);
extern __m128i invaes_jazz(__m256i key, __m128i cipher);

int8_t* string_hex_to_int8_array(const char* hex_string) {
    size_t hex_string_length = strlen(hex_string);
    size_t array_size = hex_string_length / 2;

    int8_t* array = (int8_t*)malloc(array_size * sizeof(int8_t));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    for (size_t i = 0; i < hex_string_length / 2; ++i) {
        char hex[2] = { hex_string[i * 2], hex_string[i * 2 + 1] };
        // cast to int8_t
        array[i] = (int8_t)strtol(hex, NULL, 16);
    }
    
    return array;
}

void printInt8Array(const int8_t arr[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%02x", (unsigned char)arr[i]);

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

// __m256i arr_to_u256(int8_t* arr) {
// 	return _mm_loadu_si256((__m256i *) arr);
// }

__m256i arr_to_u256(int8_t* arr) {
    __m128i* ptr = (__m128i*)arr;
    __m256i result = _mm256_set_m128i(_mm_loadu_si128(ptr + 1), _mm_loadu_si128(ptr));
    return result;
}


void u128_to_arr(__m128i value, int8_t* arr) {
    _mm_storeu_si128((__m128i *)arr, value);
}

void nou() {
	printf("nou test\n");
	
	int8_t* key = string_hex_to_int8_array("c47b0294dbbbee0fec4757f22ffeee3587ca4730c3d33b691df38bab076bc558");
	int8_t* plain = string_hex_to_int8_array("00000000000000000000000000000000");
	int8_t* cipher = string_hex_to_int8_array("46f2fb342d6f0ab477476fc501242c5f");

	printf("key: 	"); printInt8Array(key, ARRAY_SIZE_256_BIT);
	printf("plain: 	"); printInt8Array(plain, ARRAY_SIZE_128_BIT);
	printf("cipher: "); printInt8Array(cipher, ARRAY_SIZE_128_BIT);

	printf("Jasmin-generated encryption\n");
	int8_t cipher_computed[ARRAY_SIZE_128_BIT]; u128_to_arr(aes_jazz(arr_to_u256(key), arr_to_u128(plain)), cipher_computed);
	printf("computed cipher: "); printInt8Array(cipher_computed, sizeof(cipher_computed));

	// printf("Jasmin-generated decryption\n");
	// int8_t plain_computed[ARRAY_SIZE_128_BIT]; u128_to_arr(invaes_jazz(arr_to_u256(key), arr_to_u128(cipher_computed)), plain_computed);
	// printf("computed plain: "); printInt8Array(plain_computed, sizeof(plain_computed));

	free(key);
	free(plain);
	free(cipher);
}

int main() {
	nou();

	return 0;
}
