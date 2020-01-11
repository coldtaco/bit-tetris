from distutils.core import setup, Extension

setup(name='tetris', version='1.0',  \
      ext_modules=[Extension('tetris', ['tetris.c','helper.c'])])