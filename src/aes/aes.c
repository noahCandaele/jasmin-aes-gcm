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
#include <math.h>

#define RED_TEXT "\x1b[31m"
#define YELLOW_TEXT "\x1b[33m"
#define GREEN_TEXT "\x1b[32m"
#define RESET_COLOR "\x1b[0m"

#define RETURN_SUCCESS 0
#define RETURN_FAIL 1
#define RETURN_WARNING 2

#define NB_BYTES_128_BITS 16
#define NB_BYTES_192_BITS 24
#define NB_BYTES_256_BITS 32

extern __m128i aes_jazz(__m128i key, __m128i plain);
extern __m128i invaes_jazz(__m128i key, __m128i cipher);

extern __m128i aes_block_cipher_mode_jazz(__m128i key, __m128i counter, __m128i plain);
extern __m128i invaes_block_cipher_mode_jazz(__m128i key, __m128i counter, __m128i cipher);

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
	// if (array_size != strlen(hex_string) / 2) {
	// 	printf(YELLOW_TEXT "Warning: string_hex_to_int8_array: trying to convert hex string of %zu bytes to array of %zu bytes\n" RESET_COLOR, strlen(hex_string) / 2, array_size);
	// }
	
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

// void string_to_hex(char* input, char* output) {
//     size_t len = strlen(input);

//     for (size_t i = 0, j = 0; i < len; ++i, j += 2) {
//         sprintf(output + j, "%02x", input[i] & 0xFF);
//     }

//     output[len * 2] = '\0'; // Null-terminate the resulting string
// }

void string_to_hex(char* input, char* output)
{
    int loop;
    int i;

    i = 0;
    loop = 0;

    while (input[loop] != '\0') {
        sprintf((char*)(output + i), "%02X", input[loop]);
        loop += 1;
        i += 2;
    }
    //insert NULL at the end of the output string
    output[i++] = '\0';
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

void aes_block_cipher_mode(int8_t* key, int8_t* counter, int8_t* plain, int8_t* cipher_computed) {
	u128_to_arr(aes_block_cipher_mode_jazz(arr_to_u128(key), arr_to_u128(counter), arr_to_u128(plain)), cipher_computed);
}

void invaes_block_cipher_mode(int8_t* key, int8_t* counter, int8_t* cipher, int8_t* plain_computed) {
	u128_to_arr(aes_block_cipher_mode_jazz(arr_to_u128(key), arr_to_u128(counter), arr_to_u128(cipher)), plain_computed);
}

void generate_iv(int8_t* iv) {
	bool loop = true;
	// create first 64-bit part of the IV
	unsigned long long iv_part1;
	while (loop) {
		__builtin_ia32_rdrand64_step(&iv_part1);
		// verify that the first 64-bit part of the IV is not zero
		if (iv_part1 > 0xFFFFFFFFFFFFFFF) {
			loop = false;
		}
	}
	loop = true;
	uint32_t iv_part2;
	while (loop) {
		// create second 32-bit part of the IV
		__builtin_ia32_rdrand32_step(&iv_part2);
		if (iv_part2 > 0xFFFFFFF) {
			loop = false;
		}
	}

	uint32_t zeros = 0;

	char iv_char_arr[sizeof(iv_part1) * 2 + sizeof(iv_part2) * 2 + sizeof(zeros) * 2 + 1];
    sprintf(iv_char_arr, "%llx%x%08x", iv_part1, iv_part2, zeros);
	string_hex_to_int8_array(iv_char_arr, iv, NB_BYTES_128_BITS);
}

int test_aes128() {
	printf("Test AES-128\n");
	printf("\n");

	printf("Test 1\n");
	printf("\n");
	
	int8_t* key = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* plain = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* cipher = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (key == NULL || plain == NULL || cipher == NULL) {
		printf("test_aes128: allocation of key, plain or cipher failed");
		return RETURN_FAIL;
	}
	string_hex_to_int8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	string_hex_to_int8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	string_hex_to_int8_array("29C3505F571420F6402299B31A02D73A", cipher, NB_BYTES_128_BITS);

	printf("key (hex)     : "); print_int8_array(key, NB_BYTES_128_BITS);
	printf("key (ascii)   : "); print_ascii_representation(key, NB_BYTES_128_BITS);
	printf("\n");
	printf("plain (hex)   : "); print_int8_array(plain, NB_BYTES_128_BITS);
	printf("plain (ascii) : "); print_ascii_representation(plain, NB_BYTES_128_BITS);
	printf("\n");
	printf("cipher (hex)  : "); print_int8_array(cipher, NB_BYTES_128_BITS);
	printf("cipher (ascii): "); print_ascii_representation(cipher, NB_BYTES_128_BITS);
	printf("\n");

	printf("Jasmin-generated encryption\n");
	int8_t* cipher_computed = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (cipher_computed == NULL) {
		printf("test_aes128: allocation of cipher_computed failed");
		return RETURN_FAIL;
	}
	aes(key, plain, cipher_computed);
	printf("computed cipher (hex)  : "); print_int8_array(cipher_computed, NB_BYTES_128_BITS);
	printf("computed cipher (ascii): "); print_ascii_representation(cipher_computed, NB_BYTES_128_BITS);
	printf("\n");

	printf("Jasmin-generated decryption\n");
	int8_t* plain_computed = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (plain_computed == NULL) {
		printf("test_aes128: allocation of plain_computed failed");
		return RETURN_FAIL;
	}
	invaes(key, cipher_computed, plain_computed);
	printf("computed plain (hex)  : "); print_int8_array(plain_computed, NB_BYTES_128_BITS);
	printf("computed plain (ascii): "); print_ascii_representation(plain_computed, NB_BYTES_128_BITS);
	printf("\n");

	// Compare expected values with computed values
	bool plain_success = compare_int8_arrays(plain, plain_computed, NB_BYTES_128_BITS);
	if(plain_success) {
		printf("Success: plain has been computed as expected\n");
	} else {
		printf("Failure: plain has not been computed as expected\n");
		return RETURN_FAIL;
	}
	bool cipher_success = compare_int8_arrays(cipher, cipher_computed, NB_BYTES_128_BITS);
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

	int8_t* cipher_computed2 = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (cipher_computed2 == NULL) {
		printf("test_aes128: allocation of cipher_computed2 failed");
		return RETURN_FAIL;
	}
	aes(key2, plain2, cipher_computed2);

	int8_t* plain_computed2 = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (plain_computed2 == NULL) {
		printf("test_aes128: allocation of plain_computed2 failed");
		return RETURN_FAIL;
	}
	invaes(key2, cipher_computed2, plain_computed2);

	// Compare expected values with computed values
	bool plain_success2 = compare_int8_arrays(plain2, plain_computed2, NB_BYTES_128_BITS);
	if(plain_success2) {
		printf("Success: plain2 has been computed as expected\n");
	} else {
		printf("Failure: plain2 has not been computed as expected\n");
		return RETURN_FAIL;
	}
	bool cipher_success2 = compare_int8_arrays(cipher2, cipher_computed2, NB_BYTES_128_BITS);
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
	
	int8_t* key = (int8_t*)malloc(NB_BYTES_256_BITS * sizeof(int8_t));
	int8_t* plain = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* cipher = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (key == NULL || plain == NULL || cipher == NULL) {
		printf("test_aes256: allocation of key, plain or cipher failed");
		return RETURN_FAIL;
	}
	string_hex_to_int8_array("c47b0294dbbbee0fec4757f22ffeee3587ca4730c3d33b691df38bab076bc558", key, NB_BYTES_256_BITS);
	string_hex_to_int8_array("00000000000000000000000000000000", plain, NB_BYTES_128_BITS);
	string_hex_to_int8_array("46f2fb342d6f0ab477476fc501242c5f", cipher, NB_BYTES_128_BITS);

	free(key); free(plain); free(cipher);

	// TODO test not fully implemented
	return RETURN_FAIL;
}

int test_aes128_block_cipher_mode_encryption_module() {
	printf("Test AES block cipher mode of operation, encryption part, 1 module\n");

	printf("\n");
	printf("Test 1\n");

	int8_t* key = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* iv = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* plain = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* cipher = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (key == NULL || iv == NULL || plain == NULL || cipher == NULL) {
		printf("test_aes128_block_cipher_mode: allocation of key, iv, plain or cipher failed");
		return RETURN_FAIL;
	}
	// TODO remettre
	string_hex_to_int8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	string_hex_to_int8_array("f34481ec3cc627bacd5dc3fb08f273e6", iv, NB_BYTES_128_BITS);
	string_hex_to_int8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	string_hex_to_int8_array("479be376295fc8547276c4c9a76d2822", cipher, NB_BYTES_128_BITS);

	printf("Jasmin-generated encryption\n");
	int8_t* cipher_computed = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (cipher_computed == NULL) {
		printf("test_aes128: allocation of cipher_computed failed");
		return RETURN_FAIL;
	}
	aes_block_cipher_mode(key, iv, plain, cipher_computed);
	printf("computed cipher (hex)  : "); print_int8_array(cipher_computed, NB_BYTES_128_BITS);
	printf("computed cipher (ascii): "); print_ascii_representation(cipher_computed, NB_BYTES_128_BITS);
	printf("\n");

	bool cipher_success = compare_int8_arrays(cipher, cipher_computed, NB_BYTES_128_BITS);
	if(cipher_success) {
		printf("Success: cipher has been computed as expected\n");
	} else {
		printf("Failure: cipher has not been computed as expected\n");
		return RETURN_FAIL;
	}

	free(key); free(iv); free(plain); free(cipher); free(cipher_computed);

	// #########################################################
	printf("\n");
	printf("Test 2: compute IV\n");

	int8_t* key2 = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* iv2 = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* plain2 = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (key2 == NULL || iv2 == NULL || plain2 == NULL) {
		printf("test_aes128_block_cipher_mode: allocation of key2, plain2 failed");
		return RETURN_FAIL;
	}
	string_hex_to_int8_array("5468617473206D79204B756E67204675", key2, NB_BYTES_128_BITS);
	string_hex_to_int8_array("54776F204F6E65204E696E652054776F", plain2, NB_BYTES_128_BITS);

	generate_iv(iv2);

    printf("iv2: ");
	print_int8_array(iv2, NB_BYTES_128_BITS);

	printf("Jasmin-generated encryption\n");
	int8_t* cipher_computed2 = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (cipher_computed2 == NULL) {
		printf("test_aes128_block_cipher_mode: allocation of cipher_computed2 failed");
		return RETURN_FAIL;
	}
	aes_block_cipher_mode(key2, iv2, plain2, cipher_computed2);
	printf("computed cipher (hex)  : "); print_int8_array(cipher_computed2, NB_BYTES_128_BITS);
	printf("computed cipher (ascii): "); print_ascii_representation(cipher_computed2, NB_BYTES_128_BITS);
	printf("\n");

	printf("Jasmin-generated decryption\n");
	int8_t* plain_computed2 = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (plain_computed2 == NULL) {
		printf("test_aes128_block_cipher_mode: allocation of plain_computed2 failed");
		return RETURN_FAIL;
	}
	invaes_block_cipher_mode(key2, iv2, cipher_computed2, plain_computed2);
	printf("computed plain (hex)  : "); print_int8_array(plain_computed2, NB_BYTES_128_BITS);
	printf("computed plain (ascii): "); print_ascii_representation(plain_computed2, NB_BYTES_128_BITS);
	printf("\n");

	// Compare expected values with computed values
	bool plain_success2 = compare_int8_arrays(plain2, plain_computed2, NB_BYTES_128_BITS);
	if(plain_success2) {
		printf("Success: plain has been computed as expected\n");
	} else {
		printf("Failure: plain has not been computed as expected\n");
		return RETURN_FAIL;
	}

	free(key2); free(iv2); free(plain2); free(cipher_computed2); free(plain_computed2);

	return RETURN_SUCCESS;
}

int test_aes128_block_cipher_mode_encryption_full() {
	printf("Test AES block cipher mode of operation, encryption part, full\n");

	// char* plaintext_full = "On teste avec ca, on va voir si ca marche, si ca marche ca marche, si ca marche pas ca marche pas.";
	char* plaintext_full = "lorem ipsum dol";
	// char* plaintext_full = "efghijklmnopqrstuvwxyz0123456789";
	// char* plaintext_full = "efghijklmnopqrst";


	size_t len = strlen(plaintext_full);
    // char* plaintext_hex_full = (char*)malloc(len * 2 + 1); // Each character will be represented by 2 hex digits, plus 1 for null terminator
	int8_t* plaintext_hex_full = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	// string_to_hex(plaintext_full, plaintext_hex_full);
	

	string_hex_to_int8_array("54776F204F6E65204E696E652054776F", plaintext_hex_full, NB_BYTES_128_BITS);
	printf("plaintext full hex: "); print_int8_array(plaintext_hex_full, strlen(plaintext_hex_full));
	
	int8_t* key = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* iv = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	int8_t* counter = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
	if (key == NULL || iv == NULL || counter == NULL) {
		printf("test_aes128_block_cipher_mode_encryption_full: allocation failed");
		return RETURN_FAIL;
	}
	string_hex_to_int8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// generate_iv(iv);
	string_hex_to_int8_array("f34481ec3cc627bacd5dc3fb08f273e6", iv, NB_BYTES_128_BITS);

	// string_hex_to_int8_array("5468617473206D79204B756E67204675", key, NB_BYTES_128_BITS);
	// string_hex_to_int8_array("f34481ec3cc627bacd5dc3fb08f273e6", iv, NB_BYTES_128_BITS);
	// string_hex_to_int8_array("54776F204F6E65204E696E652054776F", plain, NB_BYTES_128_BITS);
	// string_hex_to_int8_array("479be376295fc8547276c4c9a76d2822", cipher, NB_BYTES_128_BITS);

	// list of cipher blocks
	int nb_cipher_blocks = ceil(strlen(plaintext_hex_full) / 16.0);
	printf("strlen(plaintext_full): %zu\n", strlen(plaintext_hex_full));
	printf("nb_cipher_blocks: %d\n", nb_cipher_blocks);
	int8_t* cipher_blocks = (int8_t*)malloc(nb_cipher_blocks * NB_BYTES_128_BITS * sizeof(int8_t));
	if (cipher_blocks == NULL) {
		printf("test_aes128_block_cipher_mode_encryption_full: allocation failed");
		return RETURN_FAIL;
	}
	// for each 128 bit block of the plaintext
	for (size_t i = 0; i < nb_cipher_blocks; i++) {
		int8_t* plain = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
		int8_t* cipher = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
		if (plain == NULL || cipher == NULL) {
			printf("test_aes128_block_cipher_mode_encryption_full: allocation failed");
			return RETURN_FAIL;
		}
		// get ith block of plaintext
		string_hex_to_int8_array(plaintext_hex_full + i * NB_BYTES_128_BITS*2*sizeof(char), plain, NB_BYTES_128_BITS);

		// print plain
		printf("plain block %zu (hex)  : ", i);
		print_int8_array(plain, NB_BYTES_128_BITS); 
		aes_block_cipher_mode(key, iv, plain, cipher);
		// add cipher to list of cipher blocks
		memcpy(cipher_blocks + i * NB_BYTES_128_BITS*sizeof(int8_t), cipher, NB_BYTES_128_BITS);
		free(plain); free(cipher);
	}
	// print cipher blocks
	for (size_t i = 0; i < nb_cipher_blocks; ++i) {
		printf("cipher block %zu: ", i);
		print_int8_array(cipher_blocks + i * NB_BYTES_128_BITS*sizeof(int8_t), NB_BYTES_128_BITS);
	}
	// decrypt cipher blocks
	int8_t* plain_computed_full = (int8_t*)malloc(nb_cipher_blocks*NB_BYTES_128_BITS * sizeof(int8_t));
	for (size_t i = 0; i < nb_cipher_blocks; ++i) {
		int8_t* plain_computed = (int8_t*)malloc(NB_BYTES_128_BITS * sizeof(int8_t));
		if (plain_computed == NULL) {
			printf("test_aes128_block_cipher_mode_encryption_full: allocation failed");
			return RETURN_FAIL;
		}
		invaes_block_cipher_mode(key, iv, cipher_blocks + i * NB_BYTES_128_BITS, plain_computed);
		memcpy(plain_computed_full + i * NB_BYTES_128_BITS, plain_computed, NB_BYTES_128_BITS);
		free(plain_computed);
	}
	// print plain computed
	for (size_t i = 0; i < nb_cipher_blocks; ++i) {
		printf("plain computed block %zu (hex)  : ", i);
		print_int8_array(plain_computed_full + i * NB_BYTES_128_BITS, NB_BYTES_128_BITS);
		printf("plain computed block %zu (ascii): ", i);
		print_ascii_representation(plain_computed_full + i * NB_BYTES_128_BITS, NB_BYTES_128_BITS);
	}
	// convert plain computed to string
	printf("full plaintext (hex)  : "); print_int8_array(plain_computed_full, nb_cipher_blocks*NB_BYTES_128_BITS);
	printf("full plaintext (ascii): "); print_ascii_representation(plain_computed_full, nb_cipher_blocks*NB_BYTES_128_BITS);
	
	// TODO free tout
	// TODO test pas complet
	return RETURN_FAIL;
}

int main() {
	print_return_status(check_cpuflags());

	printf("___________________________________\n");
	print_return_status(test_aes128());

	// printf("___________________________________\n");
	// print_return_status(test_aes256());

	printf("___________________________________\n");
	print_return_status(test_aes128_block_cipher_mode_encryption_module());

	printf("___________________________________\n");
	print_return_status(test_aes128_block_cipher_mode_encryption_full());

	return RETURN_SUCCESS;
}
