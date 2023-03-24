# Modes of operation for block cipher: CBC (Cipher Block Chaning) and CTR (Counter Mode)

desW23.c is Michael Brown's code for the basic DES permutation, in C.

## CBC
with the basic DES permutation, the code performs DES on an n-block message in CBC mode. As is standard practice, the IV should be the initial block of ciphertext---which will end up being one block longer than the plaintext

## CTR
and code that performs the same thing but in CTR mode, with a 32-bit nonce (which would be prepended to a 32-bit working counter).