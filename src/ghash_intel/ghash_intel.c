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
	__m128i tmp0, tmp1, tmp2, d, e, tmp5, c, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12;
	__m128i XMMMASK = _mm_setr_epi32(0xffffffff, 0x0, 0x0, 0x0);

	// algo 2
	c = _mm_clmulepi64_si128(a, b, 0x11);
	d = _mm_clmulepi64_si128(a, b, 0x00);
	e = _mm_shuffle_epi32(a,78);
	tmp5 = _mm_shuffle_epi32(b,78);
	e = _mm_xor_si128(e, a);
	tmp5 = _mm_xor_si128(tmp5, b);
	e = _mm_clmulepi64_si128(e, tmp5, 0x00);

	// Debug c
	// 009b5741881e078922a3f0d23e842b46
	// 009b5741881e078922a3f0d23e842b46

	// Debug d
	// 7e35eadec1d9bcc4473efe75603871d0
	// 7e35eadec1d9bcc4473efe75603871d0

	// Debug e
	// 5fafd6573c005aff045c43f3ef0d62e2
	// 5fafd6573c005aff045c43f3ef0d62e2




	e = _mm_xor_si128(e, d);
	e = _mm_xor_si128(e, c);


	// 21016bc875c7e1b261c14d54b1b13874
	// 21016bc875c7e1b261c14d54b1b13874

	tmp5 = _mm_slli_si128(e, 8);

	// 61c14d54b1b138740000000000000000
	// 61c14d54b1b138740000000000000000

	e = _mm_srli_si128(e, 8);

	// 000000000000000021016bc875c7e1b2
	// 000000000000000021016bc875c7e1b2

	
	d = _mm_xor_si128(d, tmp5);

	// 1ff4a78a706884b0473efe75603871d0
	// 1ff4a78a706884b0473efe75603871d0

	c = _mm_xor_si128(c, e);

	// 009b5741881e078903a29b1a4b43caf4
	// 009b5741881e078903a29b1a4b43caf4

	tmp7 = _mm_srli_epi32(c, 31);

	// 00000000000000010000000000000000
	// 00000000000000010000000000000000

	tmp8 = _mm_srli_epi32(c, 30);

	// 00000000000000020000000000000001
	// 00000000000000020000000000000001

	tmp9 = _mm_srli_epi32(c, 25);

	// 00000000000000440000000100000025
	// 00000000000000440000000100000025


	tmp7 = _mm_xor_si128(tmp7, tmp8);

	// 00000000000000030000000000000001
	// 00000000000000030000000000000001


	tmp7 = _mm_xor_si128(tmp7, tmp9);
	
	// 00000000000000470000000100000024
	// 00000000000000470000000100000024

	tmp8 = _mm_shuffle_epi32(tmp7, 147);

	// 00000047000000010000002400000000
	// 00000047000000010000002400000000

	tmp7 = _mm_and_si128(XMMMASK, tmp8);

	// 00000000000000000000000000000000
	// 00000000000000000000000000000000

	tmp8 = _mm_andnot_si128(XMMMASK, tmp8);
	
	// 00000047000000010000002400000000
	// 00000047000000010000002400000000


	d = _mm_xor_si128(d, tmp8);

	// 1ff4a7cd706884b1473efe51603871d0
	// 1ff4a7cd706884b1473efe51603871d0

	c = _mm_xor_si128(c, tmp7);

	// 009b5741881e078903a29b1a4b43caf4
	// 009b5741881e078903a29b1a4b43caf4

	tmp10 = _mm_slli_epi32(c, 1);

	// 0136ae82103c0f1207453634968795e8
	// 0136ae82103c0f1207453634968795e8

	d = _mm_xor_si128(d, tmp10);

	// 1ec2094f60548ba3407bc865f6bfe438
	// 1ec2094f60548ba3407bc865f6bfe438

	tmp11 = _mm_slli_epi32(c, 2);

	// 026d5d0420781e240e8a6c682d0f2bd0
	// 026d5d0420781e240e8a6c682d0f2bd0

	d = _mm_xor_si128(d, tmp11);

	// 1caf544b402c95874ef1a40ddbb0cfe8
	// 1caf544b402c95874ef1a40ddbb0cfe8

	tmp12 = _mm_slli_epi32(c, 7);

	// 4daba0800f03c480d14d8d00a1e57a00
	// 4daba0800f03c480d14d8d00a1e57a00

	d = _mm_xor_si128(d, tmp12);

	// 5104f4cb4f2f51079fbc290d7a55b5e8
	// 5104f4cb4f2f51079fbc290d7a55b5e8

	*res = _mm_xor_si128(d, c);;

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
	

 	return _mm_shuffle_epi8(tmp1, BSWAP_MASK);
}

__m128i arr_to_u128(int8_t* arr) {
	return _mm_loadu_si128((__m128i *) arr);
}

int test_1_5() {
	printf("######## Test 1_5 ########\n");
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

	return CODE_INFO;
}

int test_2_4() {
	printf("######## Test 2_4 ########\n");
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

	// a = reflect_xmm(a);
	// b = reflect_xmm(b);
	gfmul_2_4(a, b, &res);
	// res = reflect_xmm(res);

	uint8_t r[NB_BYTES_128_BITS];
	u128_to_arr(res, r);
	print_uint8_array_as_hex(r, NB_BYTES_128_BITS, false);

	return CODE_INFO;
}

int test_reflect() {
	printf("######## Test reflect ########\n");
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

	return CODE_INFO;
}

int main() {
	print_test_return_status(test_1_5());
	print_test_return_status(test_2_4());
	print_test_return_status(test_reflect());

	return CODE_SUCCESS;
}
