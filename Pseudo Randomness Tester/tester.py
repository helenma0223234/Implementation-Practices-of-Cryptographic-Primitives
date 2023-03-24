import math
import random
import scipy.special as sc
from scipy.special import erfc as erfc
from scipy.special import gammaincc
from Serial import Serial as serial

 """
    From the NIST documentation http://csrc.nist.gov/publications/nistpubs/800-22-rev1a/SP800-22rev1a.pdf

    The focus of this code is to run four different tests from the NIST document to test if they qualify as
    a pseudo random generator.

    :input      no parameter needs to be specified. the code reads in data from list.txt
    :output     each line from list.txt is calculated and examined through the four tests, the final result would be
                printed to the console
"""

def read_from_file(path):
    with open(path, 'r') as file:
        lines = []
        for line in file:
            bits = [int(x) for x in line.strip()]
            lines.append(bits)
    return lines

def write_data():
    with open("list.txt", "r") as input_file:
        i = 0
        for line in input_file:
            with open("data_" + str(i) + ".txt", "w") as output_file:
                output_file.write(line.rstrip())
            i += 1

def monobit2(line,length):
    freq_sum = 0
    for i in range(0,len(line)):
        if line[i] == 0:
            freq_sum -= 1
        else:
            freq_sum += 1
    s_obs = freq_sum/math.sqrt(length)
    p = erfc(abs(s_obs)/math.sqrt(2))
    # 1 percent level...
    return p >= 0.01

def block_freq(line, length, block_size = 128):
    blocks = math.floor(length/block_size)
    start, end, proprtion_sum = 0, block_size, 0.0
    for i in range(blocks):
        block = line[start:end]
        ones = 0
        for bit in block:
            if bit == 1:
                ones+=1
        pi = ones / block_size
        proprtion_sum += math.pow(pi-0.5, 2.0)
        start += block_size
        end += block_size       # to discard bits that doesn't fit into a bloc kat the end
    res = 4.0 * block_size * proprtion_sum
    p_val = gammaincc(blocks / 2, res/2)
    # print(p_val)
    return p_val >= 0.01

def runs_test(line, length):
    pi = 0
    for i in range(0,len(line)):
        if line[i] == 1:
            pi += 1
    pi = pi/length
    # step 2 skipped for now? we assume the test stream must have passed test 1
    v_obs = 1
    for i in range(0, len(line)-1):
        if line[i] != line[i+1]:
            v_obs+=1
    # compute p-val
    top = v_obs - 2 * length * pi * (1-pi)
    bottom = 2 * math.sqrt(2*length)*pi*(1-pi)
    p = math.erfc(abs(top)/bottom)
    return p >= 0.01

def longest_one_run_test(line, length):
    # for length of bit n < 6128, divide to size M=8 blocks, and k = 3
    combo_freq = [0,0,0,0]
    pi_probabilities = [0.2148,0.3672,0.2305,0.1875]
    cut = line
    block_number = math.floor(length/8)
    if length % 8 != 0:     # trim the extra
        rem = length % 8
        cut = cut[:-rem]
        length = len(cut)
    for i in range(0,len(cut),8):
        sub = cut[i:i+8]
        sub = tuple(sub)
        run = sub[0]
        longest = run
        for j in range(1, len(sub)):
            if sub[j-1] == 1 and sub[j] == 1:
                run += 1
            else:
                run = 1
            longest = max(run,longest)
        longest = max(run, longest)
        if longest == 0:
            combo_freq[0] += 1
        elif longest >= 4:
            combo_freq[3] += 1
        else:
            combo_freq[longest-1] += 1
    x_sqr_obs = 0
    for i in range(0, 4):
        temp = block_number*pi_probabilities[i]
        x_sqr_obs += ((combo_freq[i] - temp)*(combo_freq[i] - temp))/(temp)
    p_val = 1-sc.gammainc(float(3/2),float(x_sqr_obs/2))          # sc.gammainc(a, x)
    # print(p_val)
    return p_val > 0.01

def call_serial_test(line):
    binary_str = ''.join(str(bit) for bit in line)
    return serial.serial_test(binary_str)

def verify(line):
    length = len(line)
    count = 0
    if (monobit2(line,length)):
        count+= 1
    else:
        return False
    if block_freq(line,length):
        count+= 1
    
    # test 2: runs test
    if runs_test(line,length):
        # print("passed runs test")
        count+=1

    # test 3: Longest Run of Ones in block test
    if (longest_one_run_test(line, length)):
        count+= 1

    # test 4: serial test
    serial = call_serial_test(line)
    if serial:
        count+=1

    # print("test passed:" + str(count))
    if count < 4:
        return 0
    elif count == 4:
        return 1
    else:
        return 2

def test_controls():    # PRNG samples as controls
    controls = []
    control_count1 = 0
    control_count2 = 0
    for i in range(1000):
        l = []
        for j in range(4000):
            l.append(random.randint(0, 1))
        controls.append(l)
    print("generated 1000 strings of random bits")
    print("testing...")
    for i in range(1000):
        test = verify(controls[i])
        if test == 1 or test == 2:
            control_count1 += 1
        if test == 2:
            control_count2 += 1
    print("for the control group (1000 total):" + str(control_count1) + " is verified to be PRNG passing 4 tests")
    print("accuracy (4 passes): " + str(control_count1/ len(controls)))
    print(str(control_count2) + " is verified to be PRNG passing all 5 tests")
    print("accuracy (5 passes): " + str(control_count2 / len(controls)))

##### main function
def main():
    # generate a control group and test accuracy and Type I error
    test_controls()

    # read in the file
    lines = read_from_file("list.txt")

    # run test through samples
    count1 = 0
    count2 = 0
    for i in range(len(lines)):
        test = verify(lines[i])
        if test == 1 or test == 2:
            count1 += 1
        if test == 2:
            count2 += 1
            print("sample #" + str(i) + " MACHINE")
        if test == 0:
            print("sample #" + str(i) + " HUMAN")

    print("for list provided from class:\n" + str(count1) + " passed 4 tests")
    print(str(count2) + " passed 5 tests")

    # uncomment to write the result
    # write_data()

if __name__ == "__main__":
    main()
