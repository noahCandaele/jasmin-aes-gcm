default: main

main: main.c aes_gcm.c aes_gcm.h
	gcc main.c aes_gcm.c -o main.out -lssl -lcrypto

clean: 
	rm -fv *.out
