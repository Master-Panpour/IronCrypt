#ifndef ENCRYPTION_UTILS_H
#define ENCRYPTION_UTILS_H
#include <openssl/aes.h>

#define AES_KEY_LENGTH 256
#define AES_BLOCK_SIZE 16

/**
 * Encrypts data using AES-256
 * @param plaintext Input data to encrypt
 * @param ciphertext Output buffer for encrypted data
 * @param key Encryption key (must be 32 bytes)
 * @return 0 on success, -1 on failure
 */
int encrypt_data(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key);

/**
 * Decrypts data using AES-256
 * @param ciphertext Input encrypted data
 * @param plaintext Output buffer for decrypted data
 * @param key Decryption key (must be 32 bytes)
 * @return 0 on success, -1 on failure
 */
int decrypt_data(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key);

#endif // ENCRYPTION_UTILS_H
