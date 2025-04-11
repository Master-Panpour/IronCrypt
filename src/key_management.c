#include <stdlib.h>
#include <string.h>

/**
 * Retrieves the encryption key from the environment variable.
 * @return Pointer to the encryption key, or NULL if not set.
 */
const unsigned char* get_encryption_key() {
    const char* key = getenv("IRONCRYPT_ENCRYPTION_KEY");
    if (key == NULL) {
        return NULL; // Key not set
    }
    
    // Ensure the key is the correct length
    if (strlen(key) != 32) {
        return NULL; // Invalid key length
    }
    
    return (const unsigned char*)key;
}
