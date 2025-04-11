#include "encryption_utils.h"
#include "key_management.h"
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>

#define AES_KEY_LENGTH 256
#define AES_BLOCK_SIZE 16

// Function to encrypt data
int encrypt_data(const unsigned char *plaintext, unsigned char *ciphertext) {
    const unsigned char *key = get_encryption_key();
    if (key == NULL) {
        return -1; // Key retrieval failed
    }

    AES_KEY encryptKey;
    if (AES_set_encrypt_key(key, AES_KEY_LENGTH, &encryptKey) < 0) {
        return -1; // Key setup failed
    }

    // Encrypt the data
    AES_encrypt(plaintext, ciphertext, &encryptKey);
    return 0; // Success
}

// Function to decrypt data
int decrypt_data(const unsigned char *ciphertext, unsigned char *plaintext) {
    const unsigned char *key = get_encryption_key();
    if (key == NULL) {
        return -1; // Key retrieval failed
    }

    AES_KEY decryptKey;
    if (AES_set_decrypt_key(key, AES_KEY_LENGTH, &decryptKey) < 0) {
        return -1; // Key setup failed
    }

    // Decrypt the data
    AES_decrypt(ciphertext, plaintext, &decryptKey);
    return 0; // Success
}
