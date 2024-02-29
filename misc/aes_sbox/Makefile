default: compile

compile: jasmin main
jasmin:
	jasminc sbox.jazz -o sbox.s
main:
	gcc sbox_test.c sbox.s -o sbox_test.out

run:
	@./sbox_test.out

checks: checksafety checkCT
checksafety:
	jasminc -checksafety sbox.jazz
checkCT:
	jasminc -checkCT sbox.jazz

clean:
	@rm -fv *.out *.s
