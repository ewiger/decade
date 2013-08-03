import os
import sys
import ctypes
from ctypes import *
import antlr3c


class AntlrTree(antlr3c.ANTLR3_BASE_TREE_struct):
    pass


pAntlrTree = POINTER(AntlrTree)


class ParsingResult(Structure):
    _fields_ = [
        ('error_count', c_int),
        ('ast', pAntlrTree),
        ('line', c_int),
        ('char_pos', c_int),
        ('msg', c_char_p),
        ('token', c_char_p),
    ]


try:
    MPARSER_DIR = os.path.realpath(os.path.dirname(__file__))
    MPARSER_LIBPATH = os.path.join(MPARSER_DIR, 'mparser.so')
    library = ctypes.CDLL(MPARSER_LIBPATH)

    library.parse_matlab.argtypes = [c_char_p]
    library.parse_matlab.restype = ParsingResult

except AttributeError:
    raise ImportError('mparser shared library not found or incompatible')


if __name__ == '__main__':
    arg = os.path.join(MPARSER_DIR, 'tests', 'big_test.m')
    res = library.parse_matlab(arg)
    print res
    print res.error_count
    print res.msg
    print res.ast
    tree = res.ast.contents
    print tree._fields_
    print tree.getType
    #print tree.children.contents
    
    #print res.ast._fields_
    #pchildren =  res.ast.children
    #print pchildren.contents
