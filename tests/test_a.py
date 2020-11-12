import helper
import os

command = "'print(3)'"
a = os.popen(helper.bin_path+" -i " + command).read()
assert(a == "3\n")
