default: compile

compile: jasmin main
jasmin:
	jasminc aes_count.jazz -o aes_count.s
main:
	gcc aes_count.c aes_count.s -o aes_count.out

run:
	@./aes_count.out

clean:
	@rm -fv *.out *.s
