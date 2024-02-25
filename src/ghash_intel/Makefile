UTILS_PATH = ../utils

default: compile

compile: utils ghash_intel
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o -lm
ghash_intel:
	gcc ghash_intel.c utils.o -o ghash_intel.out -mpclmul -msse2 -lm -mssse3

run:
	@./ghash_intel.out

full:
	@echo "-------- Clean --------" 
	@make -s clean
	@echo "\n-------- Compile --------"
	@make --no-print-directory compile
	@echo "\n-------- Run --------"
	@make -s run

clean:
	@rm -fv *.out *.s *.o
