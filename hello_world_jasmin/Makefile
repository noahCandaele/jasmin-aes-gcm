#############################
##### Compile all rules #####
#############################

all: jasmin main

#################
##### Rules #####
#################

jasmin:
	jasminc hello_world.jazz -o hello_world.s

main:
	gcc main.c hello_world.s -o main.out -z noexecstack

#################################
##### Clean generated files #####
#################################

TEMP_EXTENSIONS = *.out *.s

clean: 
	rm -fv $(TEMP_EXTENSIONS)
