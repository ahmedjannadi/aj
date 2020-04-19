import os

a = os.popen("./aj tests/file.aj").read()
assert(a=="3\n")
