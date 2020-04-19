import os

command = "'print(1)'"
a = os.popen("./aj -i " + command).read()
assert(a == "1\n")

command = "'print(\"hello world\")'"
a = os.popen("./aj -i " + command).read()
assert(a == "hello world\n")

command = "'print(type(1))'"
a = os.popen("./aj -i " + command).read()
assert(a == "NUMBER\n")

command = "'print(type(1+2))'"
a = os.popen("./aj -i " + command).read()
assert(a == "NUMBER\n")

command = "'print(type(1+2+\"a\"))'"
a = os.popen("./aj -i " + command).read()
assert(a == "STRING\n")

command = "'print(type(\"a\"+1))'"
a = os.popen("./aj -i " + command).read()
assert(a == "STRING\n")

command = "'print(type(type))'"
a = os.popen("./aj -i " + command).read()
assert(a == "C_FUNCTION\n")

command = "'print(type(print))'"
a = os.popen("./aj -i " + command).read()
assert(a == "C_FUNCTION\n")

command = "'print(type(1<2))'"
a = os.popen("./aj -i " + command).read()
assert(a == "BOOL\n")

command = "'print(type(1>2))'"
a = os.popen("./aj -i " + command).read()
assert(a == "BOOL\n")

command = "'function test() print(1) end test()'"
a = os.popen("./aj -i " + command).read()
assert(a == "1\n")

command = "'function test() return 1 end print(test())'"
a = os.popen("./aj -i " + command).read()
assert(a == "1\n")

command = "'function test() function hello() print(2) end hello() end test()'"
a = os.popen("./aj -i " + command).read()
assert(a == "2\n")
