default: compile

compile: jasmin jasmin_syscall main
jasmin:
	jasminc hello_world.jazz -o hello_world.s
jasmin_syscall:
	gcc -c jasmin_syscall.c -o jasmin_syscall.o
main:
	gcc main.c hello_world.s jasmin_syscall.o -o main.out

run:
	@./main.out

checks: checksafety checkCT
checksafety:
	jasminc -checksafety hello_world.jazz
checkCT:
	jasminc -checkCT hello_world.jazz

clean:
	@rm -fv *.out *.s
