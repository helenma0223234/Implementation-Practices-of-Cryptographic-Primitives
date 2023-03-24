# Pseudorandomness Tester

## Background
Some cognitive psychologists report that human minds do not make up good randomness---and that a sequence of coin flips "made up" by a human will be statistically distinct from a sequence of actual coin flips.

## Material
`list.txt` contains a series of bit sequences that satisfy the following:
- as ASCII strings of 0 and 1
- each sequence varyies in lengths (but no more than 5000)
- one sequence per line

Half of these are "random" sequences made up by students and submitted back earlier in class.  

## What the code does

For each string, the program determines whether it was student-generated randomness or machine-generated randomness, and output the decision to the console.

Run `tester.py` in any python environment, and have `list.txt` within the same folder.

## Explanation
I selected tests from NIST's 'A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications' and implemented the suit of tests in python. Randomness is a probabilistic property, so there's no deterministic saying of a certain generated bit string is purely random; instead, one can describe the randomness of the bit string in terms of probablity. Therefore, the test implemented are all statistical test, with null hypothesis H0 stating that Data is random, and alternative hypothesis Ha that Data is not random.

The test I selected to implement include:
1) monobit test
2) block frequency test (w/ a block size of 128 bit for all strings used as input that are relative shorter)
3) runs test
4) longest one run test
5) serial test (w/ pattern length m = 16)
NOTE: serial test is implemented with code from the following repo:https://github.com/stevenang/randomness_testsuite; others are coded by myself

The decision rule of each of the above tests, or confidence level alpha (the probablity of Type I error) is 0.01, which is 1%, as specified as a general rule of thumb in the NIST document. However, in my implementation, Type II error is a bit harder to prevent and get accounted for.

Monobit test is the primary test that tests for uniformity in numbers of ones and zeros in a sequence. A random sequence should have a approximately 1/2 of its components to be 1 and the other half 0, and a test that all subsequent test depends on. Therefore, if a sequence doesn't pass the monobit test, in my implementation, such a sequence is concluded to be non-random. If it passes this test, then the rest of the test follows.

Test 2 to 5 are all tests that test some aspect of pattern/bits of 1 and 0's frequency. For example, Block frequency test calculates the proportion of ones within every 128-bit blocks from each sequence, and determine whether that proportion is approxiamately 1/2 of 128. THe runs test ermines whether the number of runs of 0 and 1 of various length has frequency as expected for a random sequence (FIST document).

The conclusion of whether a sequence is generated by a PRNG is based on numebrs of tests passed. failing the first test means an immediate conclusion of non-random, then passing either 4 of the tests (including monobit) or all 5 tests yield different results.

To test the accuracy of my implementation, I also generated 1000 sequence of bits with the PRNG in python. By the time I ran the code, 986 passed 4 tests and 941 pasted 5 tests. The ones that are not identified are Type II errors, which means that a higher standard (5 passes) lead to a more significant Type II error.