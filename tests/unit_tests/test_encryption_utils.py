import unittest
import ctypes
import os

class TestEncryptionUtils(unittest.TestCase):
    def setUp(self):
        # Load the shared library
        self.lib = ctypes.CDLL('./libencryption.so')
        
        # Define function prototypes
        self.lib.encrypt_data.argtypes = [
            ctypes.POINTER(ctypes.c_ubyte),  # plaintext
            ctypes.POINTER(ctypes.c_ubyte)   # ciphertext
        ]
        self.lib.encrypt_data.restype = ctypes.c_int
        
        self.lib.decrypt_data.argtypes = [
            ctypes.POINTER(ctypes.c_ubyte),  # ciphertext
            ctypes.POINTER(ctypes.c_ubyte)   # plaintext
        ]
        self.lib.decrypt_data.restype = ctypes.c_int

        # Set up test data
        self.key = b'thisisaverysecurekey1234567890123456'  # 32 bytes
        os.environ['IRONCRYPT_ENCRYPTION_KEY'] = self.key.decode()
        self.plaintext = b'SensitiveData12345'  # 16 bytes
        self.ciphertext = (ctypes.c_ubyte * 16)()
        self.decrypted = (ctypes.c_ubyte * 16)()

    def test_encrypt_decrypt(self):
        # Convert plaintext to ctypes
        plaintext_arr = (ctypes.c_ubyte * 16)(*self.plaintext)
        
        # Test encryption
        result = self.lib.encrypt_data(plaintext_arr, self.ciphertext)
        self.assertEqual(result, 0, "Encryption failed")
        
        # Test decryption
        result = self.lib.decrypt_data(self.ciphertext, self.decrypted)
        self.assertEqual(result, 0, "Decryption failed")
        
        # Verify decrypted matches original
        decrypted_bytes = bytes(self.decrypted)
        self.assertEqual(decrypted_bytes[:len(self.plaintext)], self.plaintext)

if __name__ == '__main__':
    unittest.main()
