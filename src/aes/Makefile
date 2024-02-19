CFLAGS = -mavx -mrdrnd -lm
# -z noexecstack: mark the resulting executable as not requiring an executable stack
# -mavx: enable AVX instructions
# -mrdrnd: m machine, rd read, rnd random
# -lm: math

JAZZFLAGS =
# -lazy-regalloc: used to avoid the error "register allocation failed" when compiling the .jazz file (too many registers used)

UTILS_PATH = ../utils

default: compile

compile: jasmin_aes jasmin_aes_block_cipher_mode utils main
jasmin_aes:
	jasminc aes.jazz -o aes.s $(JAZZFLAGS)
jasmin_aes_block_cipher_mode:
	jasminc aes_block_cipher_mode.jazz -o aes_block_cipher_mode.s $(JAZZFLAGS)
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o $(CFLAGS)
main:
	gcc aes.c aes.s aes_block_cipher_mode.s utils.o -o aes.out $(CFLAGS)

run:
	@./aes.out

checks: checksafety checkCT
checksafety:
	jasminc -checksafety aes.jazz
checkCT:
	jasminc -checkCT aes.jazz

clean:
	@rm -fv *.out *.s
