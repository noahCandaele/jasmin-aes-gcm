default: all

all: jasmin main

jasmin:
	jasminc main_function.jazz -o main_function.s

main:
	gcc test.c main_function.s -o test.out -z noexecstack -fno-stack-protector

clean: 
	rm -fv *.out *.s
