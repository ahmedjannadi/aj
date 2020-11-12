import os
import helper

command = '\"a=0 while(a<2) print(a) a++ end\"'
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "0\n1.000000\n")
