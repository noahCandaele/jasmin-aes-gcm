default: all

all: jasmin main

jasmin:
	jasminc hello_world.jazz -o hello_world.s

main:
	gcc main.c hello_world.s -o main.out -z noexecstack

clean: 
	rm -fv *.out *.s
