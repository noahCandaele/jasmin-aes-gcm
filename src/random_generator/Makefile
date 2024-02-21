LIB_SYSCALL_PATH = ../../lib/jasmin_syscall
UTILS_PATH = ../utils

default: compile

compile: random utils jasmin_syscall test_random
random:
	jasminc random.jazz -o random.s
jasmin_syscall:
	gcc -c $(LIB_SYSCALL_PATH)/jasmin_syscall.c -o jasmin_syscall.o
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o -lm
test_random:
	gcc test_random.c utils.o jasmin_syscall.o random.s -o main.out -lm

run:
	@./test_random.out

clean:
	@rm -fv *.out *.s *.o
