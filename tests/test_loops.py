import os

command = '\"a=0 while(a<2) print(a) a++ end\"'
a = os.popen("./aj -i "+command).read()
assert(a == "0\n1.000000\n")
