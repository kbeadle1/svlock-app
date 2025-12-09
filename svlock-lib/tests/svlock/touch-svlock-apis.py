#!/usr/bin/python3

from svlock import *

res = svlock_is_initialized(0)

if res != 0:
	print("error: svlock_is_initialized() failed")
