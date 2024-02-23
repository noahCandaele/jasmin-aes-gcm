#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <smmintrin.h>
#include <stdbool.h>

#include "../utils/utils.h"

extern void ghash_jazz(uint8_t* input, uint8_t* h, uint32_t i, uint8_t* out_res);
extern void ghash_xor_jazz(uint8_t* ghash_prev, uint8_t* data,  uint8_t* h, uint32_t i, uint8_t* out_res);
extern void tag_jazz(uint8_t* h, uint8_t* ghash, uint8_t* out_tag);

int test_ghash() {
	
	printf("######## Test ghash ########\n");

	uint8_t k[NB_BYTES_128_BITS];
	uint8_t p[NB_BYTES_128_BITS];
	uint8_t iv[NB_BYTES_128_BITS];
	uint8_t iv0[NB_BYTES_128_BITS];
	uint8_t c[NB_BYTES_128_BITS];
	uint8_t h[NB_BYTES_128_BITS];
	uint8_t len[NB_BYTES_128_BITS];
	
	convert_hex_string_to_uint8_array_in_order("00000000000000000000000000000000", k, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order("00000000000000000000000000000000", p, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order("00000000000000000000000000000001", iv, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order("00000000000000000000000000000000", iv0, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order("0388dace60b6a392f328c2b971b2fe78", c, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order("66e94bd4ef8a2c3b884cfa59ca342b2e", h, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array_in_order("00000000000000000000000000000080", len, NB_BYTES_128_BITS);

	uint8_t ghash1[NB_BYTES_128_BITS];
	ghash_jazz(c, h, 0, ghash1);
	print_uint8_array_as_hex_in_order(ghash1, NB_BYTES_128_BITS, false);

	uint8_t ghash2[NB_BYTES_128_BITS];
	ghash_xor_jazz(ghash1, len, h, 1, ghash2);
	print_uint8_array_as_hex_in_order(ghash2, NB_BYTES_128_BITS, false);

	uint8_t tag[NB_BYTES_128_BITS];
	tag_jazz(h, ghash2, tag);
	print_uint8_array_as_hex_in_order(tag, NB_BYTES_128_BITS, false);

	return CODE_INFO;
}

int main()
{
	print_test_return_status(test_ghash());

	return 0;
}
