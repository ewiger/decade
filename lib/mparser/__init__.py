import os
import sys
import ctypes
from ctypes import c_char_p

try:
    MPARSER_DIR = os.path.realpath(os.path.dirname(__file__))
    MPARSER_LIBPATH = os.path.join(MPARSER_DIR, 'mparser.so')
    library = ctypes.CDLL(MPARSER_LIBPATH)

    library.parse_matlab.argtypes = [c_char_p]
    library.parse_matlab.restype = c_char_p

except AttributeError:
    raise ImportError('mparser shared library not found or incompatible')


if __name__ == '__main__':
    arg = os.path.join(MPARSER_DIR, 'test')
    res = library.parse_matlab(arg)
    print res

