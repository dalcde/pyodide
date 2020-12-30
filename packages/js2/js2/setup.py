#!/usr/bin/env python

from distutils.core import setup, Extension

setup(name="fputs",
      version="1.0.0",
      ext_modules=[Extension("js2", ["js2.c"])])
