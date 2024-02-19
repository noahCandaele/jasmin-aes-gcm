LIB_SYSCALL_PATH = ../../lib/jasmin_syscall

default: compile

compile: jasmin jasmin_syscall main
jasmin:
	jasminc random.jazz -o random.s
jasmin_syscall:
	gcc -c $(LIB_SYSCALL_PATH)/jasmin_syscall.c -o jasmin_syscall.o
main:
	gcc main.c random.s jasmin_syscall.o -o main.out

run:
	@./main.out

clean:
	@rm -fv *.out *.s *.o
