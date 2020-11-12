import os
import helper

command = "\'a = 1 if(a<3) print(a) end\'"
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "1\n")

command = "\'a = 10 if(a>3) print(a) end\'"
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "10\n")

command = "\'a = 10 if(a<3) print(a) end else print(3000) end\'"
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "3000\n")

command = "\'a = 10 if(a<30) print(a) end else print(3000) end\'"
a = os.popen(helper.bin_path + " -i "+command).read()
assert(a == "10\n")
