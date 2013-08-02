#!/usr/bin/python
'''
This code will automatically generate most of the ctypes from ANTLR3 C runtime
headers.

'''
import os
import sys
import ext_path
from pyclibrary import *
from glob import glob


ANTLR3C_INCLUDE = os.path.join(
    os.path.dirname(os.path.dirname(os.path.realpath(__file__))),
    'libantlr3c-3.4', 'include',
)


def get_antlr3c_headers():
    return glob(ANTLR3C_INCLUDE + '/*')


def parse_headers(hfiles):
    p = CParser(hfiles)
    p.processAll(verbose=True)
    return p

def save(p, output_dir):
    from pprint import pprint
    for k in p.dataList:
        with file(os.path.join(output_dir, '%s.py' % k), 'w+') as output:
            print 'Saving %s' % k
            comment = """''' 
%s

This is an auto-generated ctypes file from ANTLR3 C runtime headers. Note that
editing this file is not smart! For more details check genclib.py

It should be possible to redefine things in __init__.py if necessary (right
after imports section).

wbr, yy
'''\n""" % k.upper()
            print >>output, comment
            print >>output, '%s = \\' % k.upper()
            pprint(p.defs[k], output)

if __name__ == '__main__':
    hfiles = get_antlr3c_headers()
    print('Found (%d) ANTLR3C headers, preparing to generate ctypes..' \
          % len(hfiles))
    p = parse_headers(hfiles)
    output_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'generated')
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        with open(os.path.join(output_dir, '__init__.py'), 'w+') as touched:
            pass
    save(p, output_dir)

