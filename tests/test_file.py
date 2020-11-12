import os
import helper

a = os.popen(helper.bin_path + " tests/file.aj").read()
assert(a=="3\n")
