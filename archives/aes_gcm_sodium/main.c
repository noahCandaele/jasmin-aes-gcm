#include <sodium.h>
#include <stdio.h>
#include <string.h>

#define MESSAGE "Hello, AES-GCM!"

int main(void) {
    if (sodium_init() < 0) {
        fprintf(stderr, "Error: Unable to initialize libsodium\n");
        return 1;
    }

    // Generate a random 256-bit key
    unsigned char key[crypto_aead_aes256gcm_KEYBYTES];
    randombytes_buf(key, sizeof(key));

    // Nonce should be unique for each encryption, but should never be reused with the same key
    unsigned char nonce[crypto_aead_aes256gcm_NPUBBYTES];
    randombytes_buf(nonce, sizeof(nonce));

    // Message to be encrypted
    const char *message = MESSAGE;
    size_t message_len = strlen(message) + 1; // Include the null terminator

    // Step 1: Print the original message
    printf("Step 1: Original Message: %s\n", message);

    // Allocate space for the ciphertext
    unsigned char ciphertext[message_len + crypto_aead_aes256gcm_ABYTES];

    // Step 2: Encrypt the message
    if (crypto_aead_aes256gcm_encrypt(ciphertext, NULL, (const unsigned char *)message, message_len, NULL, 0, NULL, nonce, key) != 0) {
        fprintf(stderr, "Error: Encryption failed\n");
        return 1;
    }

    printf("Step 2: Encrypted Message (Ciphertext): ");
    for (size_t i = 0; i < sizeof(ciphertext); i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // Step 3: Decrypt the ciphertext
    unsigned char decrypted[message_len];
    if (crypto_aead_aes256gcm_decrypt(decrypted, NULL, NULL, ciphertext, sizeof(ciphertext), NULL, 0, nonce, key) != 0) {
        fprintf(stderr, "Error: Decryption failed\n");
        return 1;
    }

    printf("Step 3: Decrypted Message: %s\n", decrypted);

    return 0;
}
