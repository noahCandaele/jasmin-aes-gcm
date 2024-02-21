# GCC flags:
# -lm: link with math library

default: compile

compile: utils test_utils_jazz test_utils_c
test_utils_jazz:
	jasminc test_utils.jazz -o test_utils.s
utils: utils.h
	gcc -c utils.c -o utils.o -lm
test_utils_c: utils.h
	gcc test_utils.c utils.o test_utils.s -o test_utils.out -lm

run:
	@./test_utils.out

full:
	@echo "######## Clean ########" 
	@make -s clean
	@echo "\n######## Compile ########"
	@make --no-print-directory compile
	@echo "\n######## Run ########"
	@make -s run

clean:
	@rm -fv *.out *.s *.o
