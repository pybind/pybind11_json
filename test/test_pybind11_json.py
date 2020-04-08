#!/usr/bin/python3

from __future__ import print_function
# test passed on ubuntu 18.04 python3.6

import os.path
import sys

#sys.path.append("./build")

import pyjson
d = pyjson.dummy_json()
pyjson.print_json({"value": 1})
pyjson.print_pyobject_as_json({"value": 2})
dd = pyjson.return_json_as_pyobject()
print(dd) 
assert(dd["value"] == 1)

