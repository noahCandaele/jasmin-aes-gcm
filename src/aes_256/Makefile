default: compile

compile: jasmin main
jasmin:
	jasminc aes.jazz -o aes.s
main:
	gcc aes.c aes.s -o aes.out

run:
	@./aes.out

checks: checksafety checkCT
checksafety:
	jasminc -checksafety aes.jazz
checkCT:
	jasminc -checkCT aes.jazz

clean:
	@rm -fv *.out *.s
