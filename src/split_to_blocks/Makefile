default: compile

compile: jasmin main
jasmin:
	jasminc split_to_blocks.jazz -o split_to_blocks.s
main:
	gcc split_to_blocks.c split_to_blocks.s -o split_to_blocks.out

run:
	@./split_to_blocks.out

checks: checksafety checkCT
checksafety:
	jasminc -checksafety split_to_blocks.jazz
checkCT:
	jasminc -checkCT split_to_blocks.jazz

clean:
	@rm -fv *.out *.s
