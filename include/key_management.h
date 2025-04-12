#ifndef KEY_MANAGEMENT_H
#define KEY_MANAGEMENT_H

/**
 * Retrieves the encryption key from the environment variable.
 * @return Pointer to the encryption key, or NULL if not set.
 */
const unsigned char* get_encryption_key();

#endif // KEY_MANAGEMENT_H
