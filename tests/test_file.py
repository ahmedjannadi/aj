import os

a = os.popen("./aj test.aj").read()
assert(a == "3\n")
