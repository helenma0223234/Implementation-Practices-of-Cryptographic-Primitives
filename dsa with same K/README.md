# DSA with same ephemeral k

With a given dsa public key, and two signatures that were signed with DSA private keys that uses same k,

## Prerequisite
openssl

## This code:
- calculate the signer's private key!
- and demonstrate that it works (that is, by generating signatures that verify against dsa_pub.pem), use following commands in terminal to verify:
`openssl dgst -sha1 -sign test_private_key.pem -out m1sha1.sign msg1.txt
openssl dgst -sha1 -verify dsa_pub.pem -signature m1sha1.sign msg1.txt
`

## Files included

1. hsr.c: reads in hash and signature files and converts them to BIGNUMS

2. pemtest.c: reads in dsa key file converts the elements to BIGNUMS, and generates a new keypair file with a different private key.

3. hashes, signature and original texts of the two messages

4. DSA public key generated with Openssl
