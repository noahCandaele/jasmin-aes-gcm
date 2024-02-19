# GCC flags:
# -lm: link with math library

default: compile

compile: utils test_utils
utils: utils.h
	gcc -c utils.c -o utils.o -lm
test_utils: utils.h
	gcc test_utils.c utils.o -o test_utils.out -lm

run:
	@./test_utils.out

clean:
	@rm -fv *.out *.s *.o
