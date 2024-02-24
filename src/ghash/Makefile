UTILS_PATH = ../utils

default: compile

compile: ghash utils test_ghash
ghash:
	jasminc ghash.jazz -o ghash.s
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o -lm
test_ghash:
	gcc test_ghash.c utils.o ghash.s -o test_ghash.out -lm

run:
	@./test_ghash.out

full:
	@echo "-------- Clean --------" 
	@make -s clean
	@echo "\n-------- Compile --------"
	@make --no-print-directory compile
	@echo "\n-------- Run --------"
	@make -s run

clean:
	@rm -fv *.out *.s *.o
