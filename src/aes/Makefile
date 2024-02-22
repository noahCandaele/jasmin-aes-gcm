UTILS_PATH = ../utils

default: compile

compile: jasmin utils main
jasmin:
	jasminc aes.jazz -o aes.s
main:
	gcc test_aes.c utils.o aes.s -o test_aes.out -lm
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o -lm

run:
	@./test_aes.out

full:
	@echo "-------- Clean --------" 
	@make -s clean
	@echo "\n-------- Compile --------"
	@make --no-print-directory compile
	@echo "\n-------- Run --------"
	@make -s run

checks: checksafety checkCT
checksafety:
	jasminc -checksafety aes.jazz
checkCT:
	jasminc -checkCT aes.jazz

clean:
	@rm -fv *.out *.s *.o
