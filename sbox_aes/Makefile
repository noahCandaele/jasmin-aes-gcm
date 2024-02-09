default: all

all: jasmin main

jasmin:
	jasminc init_aes_sbox.jazz.jazz -o init_aes_sbox.jazz.s

main:
	gcc aes_program.c init_aes_sbox.jazz.s -o aes_program.out -z noexecstack

clean: 
	rm -fv *.out *.s