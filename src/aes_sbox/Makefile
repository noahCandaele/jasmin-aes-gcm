default: all

all: jasmin main

jasmin:
	jasminc sbox.jazz -o sbox.s
# -lazy-regalloc is used to avoid the error "Error: register allocation failed" when compiling the .jazz file (too many registers used)

main:
	gcc sbox_test.c sbox.s -o sbox_test.out -z noexecstack

clean:
	@rm -fv *.out *.s
