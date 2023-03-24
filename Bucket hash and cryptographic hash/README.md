# Comparing Bucket Hash and Cryptographic Hash

## Can bucket hash functions work as cryptographic hash functions?  Vice versa?

I pulled the words file from /usr/share/dict/words. Since its a comprehensive list of common english words, to help with my demonstration.

### Answer

a cryptographic hash function doesn't lead to a good bucket hash function. the way cryptoraphic hash function is designed is to provide security and with three distinct properties (preimage, second image resistance and collision resistance). Cryptographic hash is not designed with speed/maximal efficiency, and empirically they run slower than a rather good bucket hash funciton. (check the time difference in hashing 100000 strings using MD5 hash function and a bucket hash function from CS10 is my code `hashing.c`)

for security reason, good cryptographic hash function also should have "avalanching" property; even a very small change to a string leads to major changes to the hashed. bucket hash function doesn't provide it, in fact, bucket hash function hashes similiar input to similiar output for efficiency reason, so for some efficeint bucket hashing, the hashing process doesn't check every bit of input, but by only checking the first couple bits, will end up with the hashed value already. In my code, i printed out first 20 pairs of bucket hash and thier input, we can see 'AA' and 'AB' are hashed to 2470 and 2471 respectively, and this is not desirable for a secure cryptographic hash function.