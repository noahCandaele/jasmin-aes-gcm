default: all

all: jasmin main

jasmin:
	jasminc init_aes_sbox.jazz -o init_aes_sbox.s

main:
	gcc aes_program.c init_aes_sbox.s -o aes_program.out -z noexecstack

clean: 
	rm -fv *.out *.s