default: compile

compile: jasmin main
jasmin:
	jasminc test.jazz -o test.s
main:
	gcc test.c test.s -o test.out

run:
	@./test.out

clean:
	@rm -fv *.out *.s *.o
