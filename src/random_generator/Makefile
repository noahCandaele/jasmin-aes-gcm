LIB_SYSCALL_PATH = ../../lib/jasmin_syscall
UTILS_PATH = ../utils

default: compile

compile: jasmin utils jasmin_syscall main
jasmin:
	jasminc random.jazz -o random.s
jasmin_syscall:
	gcc -c $(LIB_SYSCALL_PATH)/jasmin_syscall.c -o jasmin_syscall.o
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o -lm
main:
	gcc main.c utils.o jasmin_syscall.o random.s -o main.out -lm

run:
	@./main.out

clean:
	@rm -fv *.out *.s *.o
