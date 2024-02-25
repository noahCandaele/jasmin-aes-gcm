#include <wmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
#include <sys/types.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/utils.h"

void u128_to_arr(__m128i value, int8_t* arr) {
	_mm_storeu_si128((__m128i *)arr, value);
}


// https://www.intel.com/content/dam/develop/external/us/en/documents/clmul-wp-rev-2-02-2014-04-20.pdf
// Page 25 figure 5: algorithms 1 and 5
void gfmul_1_5(__m128i a, __m128i b, __m128i *res){
	__m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9;

	tmp3 = _mm_clmulepi64_si128(a, b, 0x00);
	tmp4 = _mm_clmulepi64_si128(a, b, 0x10);
	tmp5 = _mm_clmulepi64_si128(a, b, 0x01);
	tmp6 = _mm_clmulepi64_si128(a, b, 0x11);

	tmp4 = _mm_xor_si128(tmp4, tmp5);
	tmp5 = _mm_slli_si128(tmp4, 8);
	tmp4 = _mm_srli_si128(tmp4, 8);
	tmp3 = _mm_xor_si128(tmp3, tmp5);
	tmp6 = _mm_xor_si128(tmp6, tmp4);

	tmp7 = _mm_srli_epi32(tmp3, 31);
	tmp8 = _mm_srli_epi32(tmp6, 31);
	tmp3 = _mm_slli_epi32(tmp3, 1);
	tmp6 = _mm_slli_epi32(tmp6, 1);

	tmp9 = _mm_srli_si128(tmp7, 12);
	tmp8 = _mm_slli_si128(tmp8, 4);
	tmp7 = _mm_slli_si128(tmp7, 4);
	tmp3 = _mm_or_si128(tmp3, tmp7);
	tmp6 = _mm_or_si128(tmp6, tmp8);
	tmp6 = _mm_or_si128(tmp6, tmp9);

	tmp7 = _mm_slli_epi32(tmp3, 31);
	tmp8 = _mm_slli_epi32(tmp3, 30);
	tmp9 = _mm_slli_epi32(tmp3, 25);

	tmp7 = _mm_xor_si128(tmp7, tmp8);
	tmp7 = _mm_xor_si128(tmp7, tmp9);
	tmp8 = _mm_srli_si128(tmp7, 4);
	tmp7 = _mm_slli_si128(tmp7, 12);
	tmp3 = _mm_xor_si128(tmp3, tmp7);

	tmp2 = _mm_srli_epi32(tmp3, 1);
	tmp4 = _mm_srli_epi32(tmp3, 2);
	tmp5 = _mm_srli_epi32(tmp3, 7);
	tmp2 = _mm_xor_si128(tmp2, tmp4);
	tmp2 = _mm_xor_si128(tmp2, tmp5);
	tmp2 = _mm_xor_si128(tmp2, tmp8);
	tmp3 = _mm_xor_si128(tmp3, tmp2);
	tmp6 = _mm_xor_si128(tmp6, tmp3);

	*res = tmp6;
}

void gfmul_2_4(__m128i a, __m128i b, __m128i *res){
	__m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12;
	__m128i XMMMASK = _mm_setr_epi32(0xffffffff, 0x0, 0x0, 0x0);

	tmp3 = _mm_clmulepi64_si128(a, b, 0x00);
	tmp6 = _mm_clmulepi64_si128(a, b, 0x11);

	tmp4 = _mm_shuffle_epi32(a,78);
	tmp5 = _mm_shuffle_epi32(b,78);
	tmp4 = _mm_xor_si128(tmp4, a);
	tmp5 = _mm_xor_si128(tmp5, b);

	tmp4 = _mm_clmulepi64_si128(tmp4, tmp5, 0x00);
	tmp4 = _mm_xor_si128(tmp4, tmp3);
	tmp4 = _mm_xor_si128(tmp4, tmp6);

	tmp5 = _mm_slli_si128(tmp4, 8);
	tmp4 = _mm_srli_si128(tmp4, 8);
	tmp3 = _mm_xor_si128(tmp3, tmp5);
	tmp6 = _mm_xor_si128(tmp6, tmp4);

	tmp7 = _mm_srli_epi32(tmp6, 31);
	tmp8 = _mm_srli_epi32(tmp6, 30);
	tmp9 = _mm_srli_epi32(tmp6, 25);

	tmp7 = _mm_xor_si128(tmp7, tmp8);
	tmp7 = _mm_xor_si128(tmp7, tmp9);

	tmp8 = _mm_shuffle_epi32(tmp7, 147);

	tmp7 = _mm_and_si128(XMMMASK, tmp8);
	tmp8 = _mm_andnot_si128(XMMMASK, tmp8);
	tmp3 = _mm_xor_si128(tmp3, tmp8);
	tmp6 = _mm_xor_si128(tmp6, tmp7);

	tmp10 = _mm_slli_epi32(tmp6, 1);
	tmp3 = _mm_xor_si128(tmp3, tmp10);
	tmp11 = _mm_slli_epi32(tmp6, 2);
	tmp3 = _mm_xor_si128(tmp3, tmp11);
	tmp12 = _mm_slli_epi32(tmp6, 7);
	tmp3 = _mm_xor_si128(tmp3, tmp12);

	*res = _mm_xor_si128(tmp3, tmp6);
}

__m128i reflect_xmm(__m128i X)
{
	__m128i tmp1,tmp2, tmp3;

	__m128i AND_MASK = _mm_set_epi32(0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f);
	__m128i LOWER_MASK = _mm_set_epi32(0x0f070b03, 0x0d050901, 0x0e060a02, 0x0c040800);
	__m128i HIGHER_MASK = _mm_set_epi32(0xf070b030, 0xd0509010, 0xe060a020, 0xc0408000);
	__m128i BSWAP_MASK = _mm_set_epi8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);


	tmp2 = _mm_srli_epi16(X, 4);
	tmp1 = _mm_and_si128(X, AND_MASK);
	tmp2 = _mm_and_si128(tmp2, AND_MASK);
	tmp1 = _mm_shuffle_epi8(HIGHER_MASK ,tmp1);
	tmp2 = _mm_shuffle_epi8(LOWER_MASK ,tmp2);
	tmp1 = _mm_xor_si128(tmp1, tmp2);
	
	tmp3 = _mm_shuffle_epi8(tmp1, BSWAP_MASK);
	uint8_t r[NB_BYTES_128_BITS];
	u128_to_arr(tmp3, r);
	printf("\n\nDEBUG reflect_xmm X: \n");
	print_uint8_array_as_binary(r, NB_BYTES_128_BITS, false);
	printf("\n\n");

	return tmp3;
}

__m128i arr_to_u128(int8_t* arr) {
	return _mm_loadu_si128((__m128i *) arr);
}

void test_1_5() {
	printf("Test 1_5\n");
	// Absolutely sure values (NIST test case 2):
	// c = 0388dace60b6a392f328c2b971b2fe78
	// h = 66e94bd4ef8a2c3b884cfa59ca342b2e
	// res = 5e2ec746917062882c85b0685353deb7

	uint8_t c[NB_BYTES_128_BITS];
	uint8_t h[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array("0388dace60b6a392f328c2b971b2fe78", c, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("66e94bd4ef8a2c3b884cfa59ca342b2e", h, NB_BYTES_128_BITS);

	__m128i a, b, res;
	a = arr_to_u128(c);
	b = arr_to_u128(h);

	gfmul_1_5(a, b, &res);

	uint8_t r[NB_BYTES_128_BITS];
	u128_to_arr(res, r);
	print_uint8_array_as_hex(r, NB_BYTES_128_BITS, false);
}

void test_2_4() {
	printf("Test 2_4\n");
	// Absolutely sure values (NIST test case 2):
	// c = 0388dace60b6a392f328c2b971b2fe78
	// h = 66e94bd4ef8a2c3b884cfa59ca342b2e
	// res = 5e2ec746917062882c85b0685353deb7

	uint8_t c[NB_BYTES_128_BITS];
	uint8_t h[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array("0388dace60b6a392f328c2b971b2fe78", c, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("66e94bd4ef8a2c3b884cfa59ca342b2e", h, NB_BYTES_128_BITS);

	__m128i a, b, res;
	a = arr_to_u128(c);
	b = arr_to_u128(h);

	a = reflect_xmm(a);
	b = reflect_xmm(b);
	gfmul_2_4(a, b, &res);
	res = reflect_xmm(res);

	uint8_t r[NB_BYTES_128_BITS];
	u128_to_arr(res, r);
	print_uint8_array_as_hex(r, NB_BYTES_128_BITS, false);
}

void test_reflect() {
	printf("Test reflect\n");
	// c = 0388dace60b6a392f328c2b971b2fe78
	// h = 66e94bd4ef8a2c3b884cfa59ca342b2e
	// res = 5e2ec746917062882c85b0685353deb7


	uint8_t c[NB_BYTES_128_BITS];
	uint8_t h[NB_BYTES_128_BITS];
	convert_hex_string_to_uint8_array("0388dace60b6a392f328c2b971b2fe78", c, NB_BYTES_128_BITS);
	convert_hex_string_to_uint8_array("66e94bd4ef8a2c3b884cfa59ca342b2e", h, NB_BYTES_128_BITS);

	__m128i a, b, res1, res2;
	a = arr_to_u128(c);
	b = arr_to_u128(h);

	res1 = reflect_xmm(a);
	res2 = reflect_xmm(b);

	uint8_t r1[NB_BYTES_128_BITS];
	u128_to_arr(res1, r1);
	print_uint8_array_as_hex(r1, NB_BYTES_128_BITS, false);

	uint8_t r2[NB_BYTES_128_BITS];
	u128_to_arr(res2, r2);
	print_uint8_array_as_hex(r2, NB_BYTES_128_BITS, false);
}

void main() {
	test_1_5();
	test_2_4();
	test_reflect();
}
