#!/usr/bin/python3

import sys
from svlock import *

res = svlock_init_index(0, 1)
if res != 0:
    print("Failed svlock_init_index")
    sys.exit(1)
print("Passed svlock_init_index")
    
res = svlock_get_initialized(0)
if res != 1:
    print("Failed svlock_get_initialized")
    sys.exit(1)
print("Passed svlock_get_initialized")
    
res = svlock_getvalue(0)
if res != 1:
    print("Failed svlock_getvalue")
    sys.exit(1);
print("Passed svlock_getvalue")
    
res = svlock_acquire(0)
if res != 0:
    print("Failed svlock_acquire")
    sys.exit(1)
print("Passed svlock_acquire");

res = svlock_getvalue(0)
if res != 0:
    print("Failed svlock_getvalue")
    sys.exit(1)
print("Passed svlock_getvalue")

res = svlock_release(0)
if res != 0:
    print("Failed svlock_release")
    sys.exit(1)
print("Passed svlock_release")

res = svlock_getvalue(0)
if res != 1:
    print("Failed svlock_getvalue")
    sys.exit(1)
print("Passed svlock_getvalue")

res = svlock_cleanup()
if res != 0:
    print("Failed svlock_cleanup")
    sys.exit(1)
print("Passed svlock_cleanup")
