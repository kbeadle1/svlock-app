#!/bin/bash

dpkg-buildpackage -b -us -uc
make -C python -f Makefile.deb
dpkg-buildpackage -b -us -uc
#cp ../build/svlock-lib/python/svlock_wrap.c python/svlock_wrap.c
#cp ../build/svlock-lib/python/svlock.py python/svlock.py
