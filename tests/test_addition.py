import os
import helper

command = "'print(1+2)'"
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "3.000000\n")

command = "'a=1 b=2 print(a+b)'"
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "3.000000\n")

command = "'print(\"hello \"+\"world\")'"
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "hello world\n")

command = "'print(1+2+\"3\"+2+1)'"
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "3.00000033.000000\n")
