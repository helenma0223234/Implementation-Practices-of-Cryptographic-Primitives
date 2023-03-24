# Implement basic RSA in C. (no padding)

primelist.txt Download primelist.txt is a list of prime numbers 15 bits or shorter.  (Thanks to primes.umn.edu.)

## what it does

The program should:

- ask the user for the indices of p and q in this list
- then calculate n, phi
- then derive an e that works
- then calculate d 
- And then loop: asking the user for a message m, then calculating c = me mod n, then calculating cd mod n (and verifying that it's m again).

## sample output

```
read 3432 primes 
Enter indices for two primes:
1254
2001
p = 10243, q = 17401
n = 178238443, phi = 178210800
e = 7
d = 101834743
e = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 
d = 0 0 0 1 1 0 0 0 0 1 0 0 0 1 1 1 0 1 1 1 1 1 1 1 1 1 0 1 1 1 
n = 0 0 1 0 1 0 1 0 0 1 1 1 1 1 1 0 1 1 0 0 1 1 1 1 1 0 1 0 1 1 
Enter message: 
234
c = m^e mod n = 100004567
c^d mod n = 234
Enter message: 
```
