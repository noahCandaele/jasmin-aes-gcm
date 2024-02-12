default: all

all: jasmin main

jasmin:
	jasminc sbox.jazz -o sbox.s

main:
	gcc sbox_test.c sbox.s -o sbox_test.out -z noexecstack

clean:
	@rm -fv *.out *.s
