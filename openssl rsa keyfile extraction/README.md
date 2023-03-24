# Play around with RSA and openssl


## Preparation

Used openssl on the command line:
- generate a 1024-bit RSA keypair, key.pem
- examine its elements
- extract the public key as public.pem

Command used: 
```
openssl genrsa -out key.pem 1024
openssl rsa -text -in key.pem
openssl rsa -in key.pem -out public.pem -pubout
```

## what the Code does (in output)

Verify whether 
1. n = pq
2. ed = 1 mod (p-1)(q-1)
