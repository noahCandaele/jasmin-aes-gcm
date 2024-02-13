default: compile

compile: jasmin main
jasmin:
	jasminc hello_world.jazz -o hello_world.s
main:
	gcc main.c hello_world.s -o main.out

run:
	@./main.out

checks: checksafety checkCT
checksafety:
	jasminc -checksafety hello_world.jazz
checkCT:
	jasminc -checkCT hello_world.jazz

clean:
	@rm -fv *.out *.s
