#!/usr/local/bin/python
import cPickle
import io
import base64
import sys

cWiseUnpickler = cPickle.Unpickler(io.BytesIO(base64.b64decode(sys.argv[1])))
cWiseUnpickler.find_global = None

try: print cWiseUnpickler.load()
except: pass
