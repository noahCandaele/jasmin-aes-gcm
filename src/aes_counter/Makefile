LIB_SYSCALL_PATH = ../../lib/jasmin_syscall
UTILS_PATH = ../utils

default: compile

compile: jasmin utils jasmin_syscall main
jasmin:
	jasminc aes_count.jazz -o aes_count.s
main:
	gcc aes_count.c jasmin_syscall.o utils.o aes_count.s -o aes_count.out -lm
jasmin_syscall:
	gcc -c $(LIB_SYSCALL_PATH)/jasmin_syscall.c -o jasmin_syscall.o
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o -lm
run:
	@./aes_count.out

full:
	@echo "######## Clean ########" 
	@make -s clean
	@echo "\n######## Compile ########"
	@make --no-print-directory compile
	@echo "\n######## Run ########"
	@make -s run

clean:
	@rm -fv *.out *.s *.o
