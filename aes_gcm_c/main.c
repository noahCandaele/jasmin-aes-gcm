#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include "aes_gcm.h"
// TODO à enlever
#include "../secret.h"

int main() {
   printf("key test: %s\n", AES_GCM_KEY);

   int size_key = strlen(AES_GCM_KEY);
   printf("size key: %d\n", size_key);

   test_aes_openssl();

   return 0;
}


void test_aes_openssl() {
   int AES_BLOCK_SIZE = 128;
   // Input plaintext and key
   unsigned char plaintext[] = "This is a secret message";
   unsigned char key[AES_BLOCK_SIZE] = "mysecretkey12345"; // 128-bit key

   // AES variables
   AES_KEY aes_key;
   unsigned char encrypted[AES_BLOCK_SIZE];
   unsigned char decrypted[AES_BLOCK_SIZE];

   // Initialize the AES key
   AES_set_encrypt_key(key, 128, &aes_key);

   // Encrypt plaintext
   AES_encrypt(plaintext, encrypted, &aes_key);

   // Decrypt ciphertext
   AES_set_decrypt_key(key, 128, &aes_key);
   AES_decrypt(encrypted, decrypted, &aes_key);

   printf("Plaintext: %s\n", plaintext);
   printf("Encrypted: ");
   for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
      printf("%02x", encrypted[i]);
   }
   printf("\n");

   printf("Decrypted: %s\n", decrypted);
}
