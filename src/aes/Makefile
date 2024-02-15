CFLAGS = -mavx -mrdrnd -lm
# -z noexecstack: mark the resulting executable as not requiring an executable stack
# -mavx: enable AVX instructions
# -mrdrnd: m machine, rd read, rnd random

JAZZFLAGS =
# -lazy-regalloc: used to avoid the error "register allocation failed" when compiling the .jazz file (too many registers used)

default: compile

compile: jasmin_aes jasmin_aes_block_cipher_mode main
jasmin_aes:
	jasminc $(JAZZFLAGS) aes.jazz -o aes.s
jasmin_aes_block_cipher_mode:
	jasminc $(JAZZFLAGS) aes_block_cipher_mode.jazz -o aes_block_cipher_mode.s
main:
	gcc $(CFLAGS) aes.c aes.s aes_block_cipher_mode.s -o aes.out

run:
	@./aes.out

checks: checksafety checkCT
checksafety:
	jasminc -checksafety aes.jazz
checkCT:
	jasminc -checkCT aes.jazz

clean:
	@rm -fv *.out *.s
