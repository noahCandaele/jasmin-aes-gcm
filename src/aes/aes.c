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

static void check_cpuflags(void) {
	uint32_t eax = 1, ebx, ecx, edx;
	int arggl = 0;

	native_cpuid(&eax, &ebx, &ecx, &edx);

	if (!(ecx & (1 << 25))) {
	  (void) fprintf(stderr,
		"Your CPU does not seem to have the AES instruction set\n");
	  arggl += 1;
	}

	if (!(ecx & (1 << 28))) {
	  (void) fprintf(stderr,
		 "Your CPU does not seem to have the AVX instruction set\n");
	  arggl += 1;
	}

	if (arggl) {
	  (void) fprintf(stderr,
			"There is a high probability that the program triggers "
			"an \"Illegal instruction\" exception\n");
	}
}

void print_u128(__m128i in) {
    alignas(16) uint16_t v[8];
    _mm_store_si128((__m128i*)v, in);
    printf("v8_u16: %x %x %x %x,  %x %x %x %x\n", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
}

void vincent() {
	int8_t key[] 	= {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	int8_t plain[] 	= {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	int8_t cipher[] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};

	check_cpuflags();

	printf("Test vector\n");
	__m128i key_m128i  = _mm_loadu_si128((__m128i *) key);
	__m128i plain_m128i  = _mm_loadu_si128((__m128i *) plain);
	__m128i cipher_m128i = _mm_loadu_si128((__m128i *) cipher);
	// _mm_storeu_si128
	printf("key: 	"); print_u128(key_m128i);
	printf("plain: 	"); print_u128(plain_m128i);
	printf("cipher: "); print_u128(cipher_m128i);

	printf("Jasmin-generated encryption\n");
	__m128i cipher_computed = aes_jazz(key_m128i, plain_m128i);
	printf("cipher: "); print_u128(cipher_computed);

	printf("Jasmin-generated decryption\n");
	__m128i plain_computed = invaes_jazz(key_m128i, cipher_computed);
	printf("plain: 	"); print_u128(plain_computed);
}

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

void u128_to_arr(__m128i value, int8_t* arr) {
    _mm_storeu_si128((__m128i *)arr, value);
}

void nou() {
	printf("nou test\n");
	
	int8_t* key = string_hex_to_int8_array("00000000000000000000000000000000");
	int8_t* plain = string_hex_to_int8_array("f34481ec3cc627bacd5dc3fb08f273e6");
	int8_t* cipher = string_hex_to_int8_array("0336763e966d92595a567cc9ce537f5e");

	printf("key: 	"); printInt8Array(key, ARRAY_SIZE_128_BIT);
	printf("plain: 	"); printInt8Array(plain, ARRAY_SIZE_128_BIT);
	printf("cipher: "); printInt8Array(cipher, ARRAY_SIZE_128_BIT);

	printf("Jasmin-generated encryption\n");
	int8_t cipher_computed[ARRAY_SIZE_128_BIT];
	u128_to_arr(aes_jazz(arr_to_u128(key), arr_to_u128(plain)), cipher_computed);
	printf("computed cipher: "); printInt8Array(cipher_computed, sizeof(cipher_computed));

	printf("Jasmin-generated decryption\n");
	int8_t plain_computed[ARRAY_SIZE_128_BIT];
	u128_to_arr(invaes_jazz(arr_to_u128(key), arr_to_u128(cipher_computed)), plain_computed);
	printf("computed plain: "); printInt8Array(plain_computed, sizeof(plain_computed));
}

int main() {
	// vincent();

	nou();

	return 0;
}
