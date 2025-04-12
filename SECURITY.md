# IronCrypt Security Guidelines

## Encryption System

### Key Management
1. Set the encryption key as an environment variable:
   ```bash
   export IRONCRYPT_ENCRYPTION_KEY="your-32-byte-secure-key-here"
   ```
2. The key must be exactly 32 bytes (256 bits) for AES-256
3. Never commit the key to version control

### Best Practices
- Rotate keys periodically (recommended every 90 days)
- Store keys securely using:
  - Environment variables in production
  - Secret management services (AWS Secrets Manager, HashiCorp Vault, etc.)
- Restrict key access to authorized personnel only

## Secure Development

### Code Practices
- All sensitive data must be encrypted before storage
- Use the provided encryption utilities (`encryption_utils.h`)
- Validate all inputs to encryption functions

### Audit Logging
- Log all encryption/decryption operations
- Monitor for failed decryption attempts

## Incident Response
- Report any suspected key compromise immediately
- Have a key rotation procedure ready
