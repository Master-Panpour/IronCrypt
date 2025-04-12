# IronCrypt

A secure data storage and encryption system.

## Security Setup

Before running the application, you must set the encryption key:

```bash
export IRONCRYPT_ENCRYPTION_KEY="your-32-byte-secure-key-here"
```

Requirements:
- Key must be exactly 32 bytes (256 bits)
- Key should contain cryptographically secure random characters
- Never commit the key to version control

For complete security guidelines, see [SECURITY.md](SECURITY.md)

## Features
- AES-256 encryption for sensitive data
- Secure key management
- Encrypted storage backend

## Installation
...
