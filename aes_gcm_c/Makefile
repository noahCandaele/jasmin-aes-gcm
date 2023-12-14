default: aes_gcm main

main:
	gcc main.c -o main.out

aes_gcm: aes_gcm.h
	gcc aes_gcm.c

clean: 
	rm -fv *.out
