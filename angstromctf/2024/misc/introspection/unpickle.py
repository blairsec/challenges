#!/usr/local/bin/python
import pickle
import io
import base64
import sys

WiseUnpickler = pickle.Unpickler(io.BytesIO(base64.b64decode(sys.argv[1])))
WiseUnpickler.find_class = None

try: print WiseUnpickler.load()
except: pass
