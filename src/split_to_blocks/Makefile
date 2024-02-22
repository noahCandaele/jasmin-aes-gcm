UTILS_PATH = ../utils

default: compile

compile: jasmin utils main
jasmin:
	jasminc split_to_blocks.jazz -o split_to_blocks.s
main:
	gcc test_split_to_blocks.c utils.o split_to_blocks.s -o split_to_blocks.out -lm
utils:
	gcc -c $(UTILS_PATH)/utils.c -o utils.o -lm

run:
	@./split_to_blocks.out

full:
	@echo "-------- Clean --------" 
	@make -s clean
	@echo "\n-------- Compile --------"
	@make --no-print-directory compile
	@echo "\n-------- Run --------"
	@make -s run

checks: checksafety checkCT
checksafety:
	jasminc -checksafety split_to_blocks.jazz
checkCT:
	jasminc -checkCT split_to_blocks.jazz

clean:
	@rm -fv *.out *.s *.o
