default: all

all: jasmin main

jasmin:
	jasminc -lazy-regalloc init_aes_sbox.jazz -o init_aes_sbox.s
# -lazy-regalloc is used to avoid the error "Error: register allocation failed" when compiling the .jazz file (too many registers used)

main:
	gcc aes_program.c init_aes_sbox.s -o aes_program.out -z noexecstack

clean: 
	rm -fv *.out *.s