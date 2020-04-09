import os

command = "'print(1)'"
a = os.popen("./aj -i " + command).read()
assert(a == "1\n")

command = "'print(\"hello world\")'"
a = os.popen("./aj -i " + command).read()
assert(a == "hello world\n")
