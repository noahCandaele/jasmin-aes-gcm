UTILS_PATH = ../utils

default: compile

compile: jasmin utils main
jasmin:
	jasminc aes_gcm.jazz -o aes_gcm.s
main:
	gcc test_aes_gcm.c utils.o aes_gcm.s -o test_aes_gcm.out -lm
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o -lm

run:
	@./test_aes_gcm.out

full:
	@echo "-------- Clean --------" 
	@make -s clean
	@echo "\n-------- Compile --------"
	@make --no-print-directory compile
	@echo "\n-------- Run --------"
	@make -s run

clean:
	@rm -fv *.out *.s *.o
