#ifndef AES_GCM_H
#define AES_GCM_H

#include <stddef.h>

typedef struct {
    // Add necessary variables to hold state, keys, IV, etc.
} AES_GCM_Context;

int generate_key(size_t key_len);
void aes_gcm_init(AES_GCM_Context *ctx, const unsigned char *key, size_t key_len, const unsigned char *iv, size_t iv_len);
void aes_gcm_encrypt(AES_GCM_Context *ctx, const unsigned char *plaintext, size_t plaintext_len, unsigned char *ciphertext, unsigned char *tag);
void aes_gcm_decrypt(AES_GCM_Context *ctx, const unsigned char *ciphertext, size_t ciphertext_len, unsigned char *plaintext, unsigned char *tag);
void calculate_authentication_tag(AES_GCM_Context *ctx, unsigned char *tag);
int verify_authentication_tag(const unsigned char *expected_tag, const unsigned char *tag);

#endif
