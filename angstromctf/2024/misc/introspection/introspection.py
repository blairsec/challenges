#!/usr/local/bin/python
import subprocess
import sys

pkl = raw_input("Enter your introspective pickle: ")

pickle_result = subprocess.check_output([sys.executable, "unpickle.py", pkl])
cpickle_result = subprocess.check_output([sys.executable, "cunpickle.py", pkl])

print open("flag.txt").read().strip() if "pickle" in pickle_result and "cPickle" not in pickle_result and "cPickle" in cpickle_result and "pickle" not in cpickle_result else "You have much to learn."
